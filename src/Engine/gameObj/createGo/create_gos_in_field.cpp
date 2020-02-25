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

#include "create_go_by_hand.h" // tmp

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
        create_go_by_hand(GoSpecFromJson::str_2_goSpeciesId("fieldRim"),
                                GoAssemblePlanSet::str_2_goLabelId(""),
                                fieldRef.get_midMPos(),
                                fieldRef.get_midDPos(),
                                NineDirection::Center,
                                BrokenLvl::Lvl_0
                                ); 
    }
    

    //----- bioSoup ------//
    // 临时简易方案
    
    for( const auto goDataPtr : job_fieldPtr->get_bioSoupGoDataPtrs() ){
        create_go_from_goDataForCreate( goDataPtr );
    }

    //----- land majorGo in blueprint -----//
    for( const auto goDataPtr : job_fieldPtr->get_majorGoDataPtrs() ){
        create_go_from_goDataForCreate( goDataPtr );
    }

    
    for( const auto goDataPtr : job_fieldPtr->get_floorGoDataPtrs() ){
        create_go_from_goDataForCreate( goDataPtr );
    }
    
}


