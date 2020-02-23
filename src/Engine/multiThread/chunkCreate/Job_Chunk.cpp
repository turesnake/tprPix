/*
 * ======================= Job_Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#include "Job_Chunk.h"

//-------------------- CPP --------------------//
#include <functional>
#include <vector>

//-------------------- Engine --------------------//
#include "simplexNoise.h"
#include "mapEntKey.h"
#include "GoAssemblePlan.h"
#include "WindClock.h"
#include "IntVec.h"
#include "random.h"

#include "GoSpecFromJson.h"

#include "YardBlueprint.h"

#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h" // tmp




namespace jChunk_inn {//-------- namespace: jChunk_inn --------------//


    const std::vector<IntVec2> near_2_vertical_mps {
        IntVec2{ 0, -1 },
        IntVec2{ 0,  1 }
    };
    const std::vector<IntVec2> near_2_horizontal_mps{
        IntVec2{ -1,  0 },
        IntVec2{  1,  0 }
    };
    const std::vector<IntVec2> near_8_mps{
        IntVec2{ -1,  -1 },
        IntVec2{  0,  -1 },
        IntVec2{  1,  -1 },
        //--
        IntVec2{ -1,  0 },
        IntVec2{  1,  0 },
        //--
        IntVec2{ -1,  1 },
        IntVec2{  0,  1 },
        IntVec2{  1,  1 }
    };



    //- nature_floorYard 尺寸为 2f2，一旦发现 某个field 被 人造物占据
    // 直接不用生成  nature_floorGos
    bool is_near_4_field_in_artifactFieldKeys( EcoObj &ecoObjRef_, IntVec2 anyMPos_ )noexcept{
        IntVec2 mpos {};
        fieldKey_t fieldKey {};
        for( int j=0; j<2; j++ ){
            for( int i=0; i<2; i++ ){ // near 2f2 field
                mpos = anyMPos_ + IntVec2{ i*ENTS_PER_FIELD, j*ENTS_PER_FIELD };
                fieldKey = anyMPos_2_fieldKey( mpos );
                if( ecoObjRef_.is_find_in_artifactFieldKeys(fieldKey) ){
                    return true;
                }
            }
        }
        return false;
    }


}//------------- namespace: jChunk_inn end --------------//


void Job_Chunk::init()noexcept{

    this->mapEntInns.resize( ENTS_PER_CHUNK * ENTS_PER_CHUNK );
    this->job_fields.reserve( cast_2_size_t(FIELDS_PER_CHUNK * FIELDS_PER_CHUNK) );
    this->fields.reserve( cast_2_size_t(FIELDS_PER_CHUNK * FIELDS_PER_CHUNK) );

    IntVec2     tmpFieldMPos {};
    fieldKey_t  tmpFieldKey {};
    for( size_t h=0; h<cast_2_size_t(FIELDS_PER_CHUNK); h++ ){
        for( size_t w=0; w<cast_2_size_t(FIELDS_PER_CHUNK); w++ ){
            tmpFieldMPos = this->chunkMPos + IntVec2{   static_cast<int>(w)*ENTS_PER_FIELD, 
                                                        static_cast<int>(h)*ENTS_PER_FIELD };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            //---
            auto [insertIt1, insertBool1] = this->job_fields.insert({ tmpFieldKey, std::make_unique<Job_Field>( *this ) });
            tprAssert( insertBool1 );
            //---
            auto [insertIt2, insertBool2] = this->fields.insert({ tmpFieldKey, std::make_unique<MapField>(tmpFieldMPos) });
            tprAssert( insertBool2 );
        }
    }
}


void Job_Chunk::write_2_field_from_jobData(){

    IntVec2     fieldMPos   {};
    fieldKey_t  fieldKey    {};    
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk
            fieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            fieldKey = fieldMPos_2_fieldKey( fieldMPos );
            //---
            auto &jobField = *(this->job_fields.at(fieldKey).get());
            auto &field    = *(this->fields.at(fieldKey).get());
            //---
            field.set_isCrossEcoObj( jobField.is_crossEcoObj() );
            field.set_isCrossColorTable( jobField.is_crossColorTable() );
        }
    }
}



bool Job_Chunk::is_borderMapEnt( IntVec2 mposOff_ )noexcept{

    tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_CHUNK) &&
                (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_CHUNK) ); //- [0,31]

    size_t idx = cast_2_size_t(mposOff_.y * ENTS_PER_CHUNK + mposOff_.x);
    tprAssert( idx < this->mapEntInns.size() );

    //---
    sectionKey_t selfEcoKey = this->mapEntInns.at(idx).ecoObjKey;
    sectionKey_t tmpEcoKey {};
    
    // 根据 mp 在 chunk 中位置
    // 确定它的 相邻mp 检测组
    const std::vector<IntVec2> *nearMPsPtr {nullptr};
    if( (mposOff_.x==0) || (mposOff_.x==ENTS_PER_CHUNK-1) ){
        // 贴着 chunk 左右 两条垂直边
        if( (mposOff_.y==0) || (mposOff_.y==ENTS_PER_CHUNK-1) ){
            // 目标 mp 为 chunk 4个顶点之一
            return false;
        }   
        nearMPsPtr = &jChunk_inn::near_2_vertical_mps;

    }else if( (mposOff_.y==0) || (mposOff_.y==ENTS_PER_CHUNK-1) ){
        // 贴着 chunk 上下 两条横向边
        nearMPsPtr = &jChunk_inn::near_2_horizontal_mps;
    }else{
        nearMPsPtr = &jChunk_inn::near_8_mps;
    }
    
    //-----------
    IntVec2 tmpOffV {};
    size_t  tmpIdx  {};

    for( const auto &mpOff : *nearMPsPtr ){
        //-- skip outEnt --
        tmpOffV = mposOff_ + mpOff; // [0,31]
        tprAssert(  (tmpOffV.x>=0) && (tmpOffV.x<ENTS_PER_CHUNK) &&
                    (tmpOffV.y>=0) && (tmpOffV.y<ENTS_PER_CHUNK) );
        //-- do check --
        tmpIdx = static_cast<size_t>(tmpOffV.y * ENTS_PER_CHUNK + tmpOffV.x);

        tmpEcoKey = this->mapEntInns.at(tmpIdx).ecoObjKey;
        if( selfEcoKey != tmpEcoKey ){
            return true;
        }
    }

    return false;
}





void Job_Chunk::create_field_goSpecDatas(){

    IntVec2         fieldMPos {};
    fieldKey_t      fieldKey {};
    size_t          fieldUWeight {};
    IntVec2         entMPos {};
    mapEntKey_t     mapEntKey {};
    
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk (8*8)

            fieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            fieldKey = fieldMPos_2_fieldKey(fieldMPos);
            //---
            auto &fieldRef = *(this->fields.at(fieldKey));
            auto &job_fieldRef = *(this->job_fields.at(fieldKey));
            fieldUWeight = fieldRef.get_uWeight();
            EcoObj &ecoObjRef = esrc::get_ecoObjRef( fieldRef.get_ecoObjKey() );

        
            //----------------------//
            //     bioSoup
            //----------------------//
            // 测试用
            
            for( int j=0; j<ENTS_PER_FIELD; j++ ){
                for( int i=0; i<ENTS_PER_FIELD; i++ ){ // each mapent in field

                    entMPos = fieldMPos + IntVec2{ i, j };
                    mapEntKey = mpos_2_key( entMPos );

                    Job_MapEnt &mp = this->getnc_mapEntInnRef( entMPos - this->chunkMPos );

                    // 简易测试版
                    // 在一个范围内，就被判断为 水岸，先生产统一的 mp-go 单位
                    //
                    //if( mp.alti.val > -5.0 && mp.alti.val < 5.0 ){   
                    if( mp.alti.val > -13.0 && mp.alti.val < 13.0 ){    

                        auto goDataUPtr = std::make_unique<GoDataForCreate>();

                        GoDataForCreate::assemble_new_goDataForCreate(  *goDataUPtr,
                                                                        mpos_2_midDPos( mp.mpos ),
                                                                        GoSpecFromJson::str_2_goSpeciesId("bioSoup"),
                                                                        GoAssemblePlanSet::str_2_goLabelId(""),
                                                                        NineDirection::Center,
                                                                        BrokenLvl::Lvl_0,
                                                                        calc_simple_uWeight( mp.mpos )
                                                                    );

                        
                        //goDataUPtr->goAltiRangeLabel = GoAltiRangeLabel::Default; // tmp
                        //goDataUPtr->goSpeciesId = GoSpecFromJson::str_2_goSpeciesId("bioSoup");
                        //goDataUPtr->dpos = mpos_2_midDPos( mp.mpos );
                        //goDataUPtr->direction = NineDirection::Center;  // 暂时无用
                        //goDataUPtr->brokenLvl_or_floorGoLayer = FloorGoLayer::L_4; // 暂时无用
                        

                        job_fieldRef.insert_2_bioSoupGoDatas( std::move(goDataUPtr) );

                    }
                }
            }
            job_fieldRef.copy_bioSoupGoDataPtrs();
            
                            
            //----------------------//
            //    artifact gos
            //----------------------//
            // 数据存储在 ecoobj 实例中
            // 只需简单地将指针 复制到 chunk 容器里
            for( int j=0; j<ENTS_PER_FIELD; j++ ){
                for( int i=0; i<ENTS_PER_FIELD; i++ ){ // each mapent in field
                    entMPos = fieldMPos + IntVec2{ i, j };
                    mapEntKey = mpos_2_key( entMPos );

                    // skip mapent in water
                    if( !this->getnc_mapEntInnRef( entMPos - this->chunkMPos ).alti.is_land() ){
                        continue;
                    }

                    //-- majorGos --
                    if( auto ret=ecoObjRef.find_majorGoDataForCreatePtr(mapEntKey); ret.has_value() ){
                        job_fieldRef.insert_2_majorGoDataPtrs( ret.value() );
                    }
                    //-- floorGos --
                    if( auto ret=ecoObjRef.find_floorGoDataForCreatePtr(mapEntKey); ret.has_value() ){
                        job_fieldRef.insert_2_floorGoDataPtrs( ret.value() );
                    }
                }
            }

                // debug 跳过 nature go 的创建
                //continue;
            

            //----------------------//
            //   nature floorGos ( 2f2-yard )
            //----------------------//
            // nature_floorYard 强制为 2f2 尺寸
            if( (h%2==0) && (w%2==0) ){
                
                // 暂时关闭这个功能，现在，不管是否已经生成 人造物 floorgo，
                // 都会再次生成 nature floorgo 
                //if( !jChunk_inn::is_near_4_field_in_artifactFieldKeys(ecoObjRef, fieldMPos) ){

                    blueprint::build_natureYard_floorGoDatasForCreate( 
                                        job_fieldRef.get_nature_floorGoDatas(),
                                        ecoObjRef.get_natureFloorYardId(),
                                        fieldMPos,
                                        fieldUWeight,

                                        [this, &ecoObjRef]( IntVec2 mpos_ )->bool { // f_is_correct_density_
                                            Density density = this->getnc_mapEntInnRef( mpos_ - this->chunkMPos ).density;
                                            return ecoObjRef.is_find_in_natureFloorDensitys(density);
                                        }

                                        );

                    job_fieldRef.copy_nature_floorGoDataPtrs();
                //}

            }

            //----------------------//
            //   nature majorGos
            //----------------------//
            // 说明 此 field 被覆盖了 人造物数据，不需要生成任何 nature 数据
            if( ecoObjRef.is_find_in_artifactFieldKeys( fieldKey ) ){
                continue;
            }
            
            const auto &densityPool = ecoObjRef.get_densityPool( fieldRef.get_density() );
            if( !densityPool.isNeed2Apply( fieldUWeight ) ){
                continue;
            }

            auto natureMajorYardId = densityPool.apply_a_yardBlueprintId( fieldUWeight );
            blueprint::build_natureYard_majorGoDatasForCreate( 
                                        job_fieldRef.get_nature_majorGoDatas(),
                                        natureMajorYardId,
                                        fieldMPos,
                                        fieldUWeight,
                                        [this]( IntVec2 mpos_ )->bool { // f_is_mapent_land_
                                            return this->getnc_mapEntInnRef( mpos_ - this->chunkMPos ).alti.is_land();
                                        }
                                        );

            job_fieldRef.copy_nature_majorGoDataPtrs();
        }
    } //- each field in chunk (8*8)

}



