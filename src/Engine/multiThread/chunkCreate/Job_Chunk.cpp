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

//-------------------- Engine --------------------//
#include "simplexNoise.h"
#include "mapEntKey.h"
#include "MultiGoMesh.h"
#include "WindClock.h"

#include "GoSpecFromJson.h"

#include "YardBlueprint.h"

#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h" // tmp




namespace jChunk_inn {//-------- namespace: jChunk_inn --------------//

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

    this->mapEntInns.resize( (ENTS_PER_CHUNK+2) * (ENTS_PER_CHUNK+2) );// 34*34 mapents
    this->job_fields.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );
    this->fields.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );

    IntVec2     tmpFieldMPos {};
    fieldKey_t  tmpFieldKey {};
    for( size_t h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( size_t w=0; w<FIELDS_PER_CHUNK; w++ ){
            tmpFieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            //---
            auto outPair1 = this->job_fields.insert({ tmpFieldKey, std::make_unique<Job_Field>() });
            tprAssert( outPair1.second );
            //---
            auto outPair2 = this->fields.insert({ tmpFieldKey, std::make_unique<MapField>(tmpFieldMPos) });
            tprAssert( outPair2.second );
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




// param: mposOff_ [0, 31]
bool Job_Chunk::is_borderMapEnt( IntVec2 mposOff_ )noexcept{

    tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_CHUNK) &&
                (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_CHUNK) ); //- [0,31]

    IntVec2 offV = mposOff_ + IntVec2{1,1}; //- 现在的 mapEntInns 外凸了一圈 mapent [1,32]
    size_t idx = cast_2_size_t(offV.y * (ENTS_PER_CHUNK+2) + offV.x);
    tprAssert( idx < this->mapEntInns.size() );

    //---
    sectionKey_t selfEcoKey = this->mapEntInns.at(idx).ecoObjKey;
    sectionKey_t tmpEcoKey {};
    
    IntVec2 tmpOffV {};
    size_t  tmpIdx  {};

    //-- nearby_9_mapent --
    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            
            //-- skip self --
            if( (j==0) && (i==0) ){
                continue;
            }
            //-- skip outEnt --
            tmpOffV = offV + IntVec2{i,j}; // [0,33]

            tprAssert(  (tmpOffV.x>=0) && (tmpOffV.x<ENTS_PER_CHUNK+2) &&
                        (tmpOffV.y>=0) && (tmpOffV.y<ENTS_PER_CHUNK+2) ); //- [0,34]

            //-- do check --
            tmpIdx = static_cast<size_t>(tmpOffV.y * (ENTS_PER_CHUNK+2) + tmpOffV.x);

            tmpEcoKey = this->mapEntInns.at(tmpIdx).ecoObjKey;
            if( selfEcoKey != tmpEcoKey ){
                return true;
            }

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
            

            //----------------------//
            //   nature floorGos ( 2f2-yard )
            //----------------------//
            // nature_floorYard 强制为 2f2 尺寸
            if( (h%2==0) && (w%2==0) ){

                if( !jChunk_inn::is_near_4_field_in_artifactFieldKeys(ecoObjRef, fieldMPos) ){

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
                }

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



