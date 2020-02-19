/*
 * ================= create_gos_in_field.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */



//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_field.h"
#include "config.h"
#include "chunkKey.h"
#include "dyParams.h"
#include "GameObj.h"
#include "create_goes.h"
#include "GoSpecFromJson.h"

#include "esrc_field.h"
#include "esrc_ecoObj.h"
#include "esrc_gameObj.h" 
#include "esrc_time.h" 
#include "esrc_chunk.h" 
#include "esrc_job_chunk.h"



/* ===========================================================
 *                create_gos_in_field     
 * -----------------------------------------------------------
 * only called in chunkCreate
 * all kinds of gos 
 * 
 *    当蓝图系统建立完成后，这个函数中的一些地方将进行大调整
 * 
 */
void create_gos_in_field(   fieldKey_t      fieldKey_, 
                            const Chunk     &chunkRef_,
                            const Job_Chunk &job_chunkRef_ ){

    const auto &fieldRef = esrc::get_field(fieldKey_);
    const auto *job_fieldPtr = job_chunkRef_.get_job_fieldPtr(fieldKey_);

    //----- ground go ------//
    {
        //--- dyParam ---//
        DyParam dyParam {};
        auto gUPtr = std::make_unique<DyParams_GroundGo>();
        gUPtr->fieldUWeight = fieldRef.get_uWeight();
        gUPtr->job_fieldPtr = job_fieldPtr;
        dyParam.insert_ptr<DyParams_GroundGo>( gUPtr.get() );
        //--- 
        gameObjs::create_a_Go(  GoSpecFromJson::str_2_goSpeciesId( "groundGo" ),
                                    fieldRef.get_midDPos(),
                                    dyParam );
    }


    //----- fieldRim go [-DEBUG-] ------//
    //  显示 map 坐标框
    bool isFieldRimGoCreate { false };
    if( isFieldRimGoCreate ){
        gameObjs::create_a_Go(  GoSpecFromJson::str_2_goSpeciesId( "fieldRim" ),
                                fieldRef.get_midDPos(),
                                emptyDyParam );
    }
    



    //----- riverBank ------//
    // 临时简易方案
    
    for( const auto goDataPtr : job_fieldPtr->get_riverBankGoDataPtrs() ){
        //--- dyParam ---//
        DyParam dyParam {};
        auto fUPtr = std::make_unique<DyParams_RiverBank>();
        fUPtr->mapEntUWeight = job_fieldPtr->getnc_mapEntUWeight( dpos_2_mpos(goDataPtr->dpos) - chunkRef_.get_mpos() );

        dyParam.insert_ptr<DyParams_RiverBank>( fUPtr.get() );
        //---
        gameObjs::create_a_Go(  goDataPtr->goSpeciesId,
                                goDataPtr->dpos,
                                dyParam ); 
    }
    
    




    //----- land majorGo in blueprint -----//
    for( const auto goDataPtr : job_fieldPtr->get_majorGoDataPtrs() ){

        // dir / brokenLvl 这2个数据 暂时未被使用
        // ...

        //--- dyParam ---//
        DyParam dyParam {};
        auto fUPtr = std::make_unique<DyParams_Blueprint>();
        fUPtr->goDataPtr = goDataPtr;
        fUPtr->mapEntUWeight = job_fieldPtr->getnc_mapEntUWeight( dpos_2_mpos(goDataPtr->dpos) - chunkRef_.get_mpos() );

        dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
        //---

        gameObjs::create_a_Go(  goDataPtr->goSpeciesId,
                                goDataPtr->dpos,
                                dyParam ); 

    }

    
    for( const auto goDataPtr : job_fieldPtr->get_floorGoDataPtrs() ){

        //  brokenLvl 这2个数据 暂时未被使用
        // ...

        //--- dyParam ---//
        DyParam dyParam {};
        auto fUPtr = std::make_unique<DyParams_Blueprint>();
        fUPtr->goDataPtr = goDataPtr;
        //fUPtr->mapEntUWeight = job_fieldPtr->getnc_mapEntUWeight( dpos_2_mpos(goDataPtr->dpos) - chunkRef_.get_mpos() );
        fUPtr->mapEntUWeight = 12345;

        dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
        //---

        gameObjs::create_a_Go(  goDataPtr->goSpeciesId,
                                goDataPtr->dpos,
                                dyParam ); 

    }



}


