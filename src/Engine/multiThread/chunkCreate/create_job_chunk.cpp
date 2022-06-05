/*
 * =================== create_job_chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build job_chunk
 * ----------------------------
 */
#include "pch.h"

//-------------------- CPP --------------------//
#include <cstring>

//-------------------- Engine --------------------//
#include "jobs_all.h"
#include "Job.h"
#include "simplexNoise.h"
#include "MapTexture.h" 
#include "Quad.h"
#include "random.h"
#include "EcoObjBorder.h"

#include "esrc_job_chunk.h"
#include "esrc_gameSeed.h"
#include "esrc_field.h"
#include "esrc_ecoObj.h"

#include "Job_MapEnt.h"
#include "Job_Field.h"




namespace bcd_inn {//----------- namespace: bcd_inn ----------------//

    //-- 不要随意建立 static数据，本文件的代码，会被数个 job线程 调用 --

    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    const std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },                                // 0: LeftBottom
        IntVec2{ ENTS_PER_SECTION<>, 0 },                 // 1: RightBottom
        IntVec2{ 0, ENTS_PER_SECTION<> },                 // 2: LeftTop
        IntVec2{ ENTS_PER_SECTION<>, ENTS_PER_SECTION<> }   // 3: RightTop
    };


    //===== funcs =====//
    void calc_job_chunk( Job_Chunk &job_chunk_ );

    void colloect_nearFour_ecoObjDatas( std::vector<std::unique_ptr<EcoObj_ReadOnly>> &container_, 
                                        IntVec2 anyMPos_ );

    void assign_mapent_to_nearFour_ecoObjs_2(   const EcoObjBorder *currentEcoObjBorderPtr_,
                                                IntVec2 sectionMPos_,
                                                std::vector<std::unique_ptr<EcoObj_ReadOnly>> &container_,     
                                                Job_MapEnt &mapEnt_ );

    
}//-------------- namespace: bcd_inn end ----------------//



/* ===========================================================
 *                create_job_chunk_main
 * -----------------------------------------------------------
 * 在未来，这个函数需要写进 atom 函数中 ...
 */
void create_job_chunk_main( const Job &job_ ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    const auto *jobParamPtr = job_.get_param<ArgBinary_Create_Job_Chunk>();

    //--------------------------//
    //      create job_chunk
    //--------------------------//
    Job_Chunk &job_chunkRef = esrc::atom_insert_new_job_chunk( jobParamPtr->chunkKey );

    //------------------------------//
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoObj )
    //------------------------------//
    // 已经在 主线程 chunkCreate_1_push_job() 中 提前完成

    //--------------------------//
    //      calc job_chunk
    //--------------------------//
    bcd_inn::calc_job_chunk( job_chunkRef );
    

    //--------------------------//
    //-- job_chunk 数据计算完成后，向 状态表 添加一个元素
    //   以此来提醒 主线程，这个 chunk 数据准备好了
    //--------------------------//
    esrc::atom_push_back_2_job_chunkFlags( jobParamPtr->chunkKey );

}


namespace bcd_inn {//----------- namespace: bcd_inn ----------------//



void calc_job_chunk( Job_Chunk &job_chunkRef_ ){

    IntVec2 currentChunkMPos = job_chunkRef_.get_chunkMPos();
    chunkKey_t currentChunkKey = job_chunkRef_.get_chunkKey();
      
    //------------------------//
    //   job_chunk.mapEntInns
    //------------------------//
    std::vector<std::unique_ptr<EcoObj_ReadOnly>> nearFour_ecoObjDatas {};
    bcd_inn::colloect_nearFour_ecoObjDatas( nearFour_ecoObjDatas, currentChunkMPos ); 

    // 本chunk 所在 ecoObj 的 border 指针
    // 但 本chunk 最终不一定会被 划分给 此ecoobj 
    IntVec2 sectionMPos = anyMPos_2_sectionMPos( currentChunkMPos );
    sectionKey_t sectionKey = sectionMPos_2_sectionKey( sectionMPos );
    std::unique_ptr<EcoObj_ReadOnly> ecoReadOnlyUPtr = esrc::get_ecoObj_readOnly(sectionKey);


    // 补完 job_MapEnt 的下半部分 init 工作
    IntVec2     mposOff {};
    for( int h=0; h<ENTS_PER_CHUNK<>; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK<>; w++ ){
            mposOff = IntVec2{ w, h };
            Job_MapEnt &mapEntRef = job_chunkRef_.getnc_mapEntInnRef(mposOff);
            assign_mapent_to_nearFour_ecoObjs_2(ecoReadOnlyUPtr->ecoObjBorderPtr,
                                                sectionMPos,
                                                nearFour_ecoObjDatas, 
                                                mapEntRef );
        }
    }

    //------------------------//
    //      fieldKeys
    //------------------------//
    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys，only used inner
    fieldKeys.reserve( FIELDS_PER_CHUNK<size_t> * FIELDS_PER_CHUNK<size_t> ); // reserve FIRST !!!

    IntVec2    tmpFieldMPos {};
    fieldKey_t tmpFieldKey {};
    IntVec2    fieldNodeOff {};

    for( int h=0; h<FIELDS_PER_CHUNK<>; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK<>; w++ ){ //- each field in chunk (8*8)
            tmpFieldMPos = IntVec2{ currentChunkMPos.x + w*ENTS_PER_FIELD<>, 
                                    currentChunkMPos.y + h*ENTS_PER_FIELD<> };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            fieldKeys.push_back(tmpFieldKey);

            //--- copy nodeMapEnt datas to field --
            auto &field = job_chunkRef_.getnc_fieldRef(tmpFieldKey);
            fieldNodeOff = dpos_2_mpos( field.get_nodeDPos() ) - anyMPos_2_chunkMPos(field.get_mpos());
            const auto &mapEntInnRef = job_chunkRef_.getnc_mapEntInnRef( fieldNodeOff );
            field.init_ecoObjKey( mapEntInnRef.get_ecoObjKey() );
            field.init_colorTableId( mapEntInnRef.get_colorTableId() );
            field.init_density( mapEntInnRef.get_density() );
            field.init_nodeMapAlti( mapEntInnRef.get_alti() );
            field.init_uWeight( static_cast<double>( mapEntInnRef.get_uWeight() ) );
        }
    }

    //------------
    IntVec2         tmpEntMPos {};
    //MapAltitude     minFieldAlti {};  
    //MapAltitude     maxFieldAlti {};

    //--- just 32 * 32 mapents in a chunk ---
    for( const auto &fieldKey : fieldKeys ){ //- each field key

        tmpFieldMPos = fieldKey_2_mpos(fieldKey);

        MapAltitude minFieldAlti { 100.0  };
        MapAltitude maxFieldAlti { -100.0 };

        for( int eh=0; eh<ENTS_PER_FIELD<>; eh++ ){
            for( int ew=0; ew<ENTS_PER_FIELD<>; ew++ ){ //- each ent in field

                tmpEntMPos = tmpFieldMPos + IntVec2{ ew, eh };
                mposOff = tmpEntMPos - currentChunkMPos;

                Job_MapEnt &mapEntRef = job_chunkRef_.getnc_mapEntInnRef(mposOff);
                //----- field min/max alti -----
                MapAltitude mapAlti = mapEntRef.get_alti();
                if( mapAlti < minFieldAlti ){ minFieldAlti = mapAlti; }
                if( mapAlti > maxFieldAlti ){ maxFieldAlti = mapAlti; }
                //--- skip unborder ent ---//
                mapEntRef.init_isEcoBorder( job_chunkRef_.is_mapEnt_a_ecoBorder(mposOff) );
                //--- insert entInnPtr ---//
                job_chunkRef_.insert_a_entInnPtr_2_field( fieldKey, IntVec2{ew, eh}, &mapEntRef );
            }
        }//- each ent in field end -- 
        //----- field data -----//
        job_chunkRef_.set_field_min_max_altis( fieldKey, minFieldAlti, maxFieldAlti );// only once

    }//-- each field key end --

    job_chunkRef_.write_2_field_from_jobData();
    // Now，the Fields all inited !!!

    //------------------------//
    //  job_field.goSpecDatas
    //------------------------//
    job_chunkRef_.create_field_goSpecDatas();
}



void colloect_nearFour_ecoObjDatas( std::vector<std::unique_ptr<EcoObj_ReadOnly>> &container_,
                                    IntVec2 anyMPos_ ){
    IntVec2 sectionMPos = anyMPos_2_sectionMPos( anyMPos_ );
    sectionKey_t tmpKey {};
    container_.clear();
    for( const auto &whOff : bcd_inn::quadSectionKeyOffs ){
        tmpKey = sectionMPos_2_sectionKey( sectionMPos + whOff );
        container_.push_back( esrc::get_ecoObj_readOnly( tmpKey ) );
    }
}



void assign_mapent_to_nearFour_ecoObjs_2(   const EcoObjBorder *currentEcoObjBorderPtr_,
                                            IntVec2 sectionMPos_,
                                            std::vector<std::unique_ptr<EcoObj_ReadOnly>> &container_, 
                                            Job_MapEnt &mapEnt_ ){

    const EcoObj_ReadOnly *ecoReadOnlyPtr {nullptr};    
    NineDirection dir = currentEcoObjBorderPtr_->assign_mapent_to_nearFour_ecoObjs_dir( mapEnt_.get_mpos() - sectionMPos_ );
    switch (dir){
        case NineDirection::LeftBottom:     ecoReadOnlyPtr =  container_.at(0).get(); break;
        case NineDirection::RightBottom:    ecoReadOnlyPtr =  container_.at(1).get(); break;
        case NineDirection::LeftTop:        ecoReadOnlyPtr =  container_.at(2).get(); break;
        case NineDirection::RightTop:       ecoReadOnlyPtr =  container_.at(3).get(); break;
        default:
            tprAssert(0);
            break;
    }

    // 正式 获取数据
    mapEnt_.init_ecoObjKey( ecoReadOnlyPtr->sectionKey );
    mapEnt_.init_colorTableId( ecoReadOnlyPtr->colorTableId );
    mapEnt_.init_density( Density{  mapEnt_.get_mpos(), 
                                    ecoReadOnlyPtr->densitySeaLvlOff,
                                    ecoReadOnlyPtr->densityDivideValsPtr } );
}



}//-------------- namespace: bcd_inn end ----------------//

