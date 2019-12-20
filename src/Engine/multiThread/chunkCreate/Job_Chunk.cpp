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


//-------------------- Engine --------------------//
#include "simplexNoise.h"
#include "mapEntKey.h"
#include "MultiGoMesh.h"
#include "WindClock.h"

#include "GoSpecFromJson.h"

#include "YardBlueprint.h"

#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h" // tmp

//-------------------- Script --------------------//


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

    sectionKey_t    ecoObjKey {};
    IntVec2         mposOff {}; // from chunk left-bottom
    IntVec2         tmpFieldMPos {};
    fieldKey_t      tmpFieldKey {};
    size_t          randUValOff {};
    animSubspecId_t subSpecId {};
    size_t             windDelayIdx {};

    IntVec2         tmpEntMPos {};
    mapEntKey_t     tmpMapEntKey {};

    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk (8*8)

            tmpFieldMPos = this->chunkMPos + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            //---
            auto &fieldRef = *(this->fields.at(tmpFieldKey));
            auto &job_fieldRef = *(this->job_fields.at(tmpFieldKey));

            if( fieldRef.is_land() ){ // skip water-field

                ecoObjKey = fieldRef.get_ecoObjKey();
                EcoObj &ecoObjRef = esrc::get_ecoObjRef(ecoObjKey);

                //----------------------//
                //  生成 蓝图用 go数据
                //----------------------//
                for( int j=0; j<ENTS_PER_FIELD; j++ ){
                    for( int i=0; i<ENTS_PER_FIELD; i++ ){ // each mapent

                        tmpEntMPos = tmpFieldMPos + IntVec2{ i, j };
                        tmpMapEntKey = mpos_2_key( tmpEntMPos );

                        //--  再筛选掉 水域之外的 mp-go --
                        mposOff = tmpEntMPos - this->chunkMPos;
                        auto &mapEntInnRef = this->getnc_mapEntInnRef( mposOff );
                        if( !mapEntInnRef.alti.is_land() ){
                            continue;
                        }

                        //-- majorGos --
                        if( auto ret=ecoObjRef.find_majorGoDataForCreatePtr(tmpMapEntKey); ret.has_value() ){
                            job_fieldRef.insert_2_blueprint_majorGoDatas( ret.value() );
                        }
                        //-- floorGos --
                        if( auto ret=ecoObjRef.find_floorGoDataForCreatePtr(tmpMapEntKey); ret.has_value() ){
                            job_fieldRef.insert_2_blueprint_floorGoDatas( ret.value() );
                        }
                    }
                }


                //----------------------//
                //   nature pools (yard)
                //----------------------//
                // 说明 此 field 被覆盖了 人造物数据，不需要生成 nature 数据
                if( ecoObjRef.is_find_in_artifactFieldKeys( tmpFieldKey ) ){
                    continue;
                }
                
                const auto &densityPool = ecoObjRef.get_densityPool( fieldRef.get_density() );
                if( !densityPool.isNeed2Apply( fieldRef.get_uWeight() ) ){
                    continue;
                }

                blueprint::yardBlueprintId_t yardId = densityPool.apply_a_yardBlueprintId( fieldRef.get_uWeight() );
                blueprint::YardBlueprint &yardRef = blueprint::YardBlueprintSet::get_yardBlueprintRef( yardId );


                blueprint::build_yard_goDatasForCreate( job_fieldRef.get_nature_majorGoDatas(),
                                                        job_fieldRef.get_nature_floorGoDatas(),
                                                        yardId,
                                                        tmpFieldMPos,
                                                        fieldRef.get_uWeight() );

                job_fieldRef.copy_nature_goDataPtrs();

            }//-- if field is land



        }
    } //- each field in chunk (8*8)

}



