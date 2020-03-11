/*
 * ================= create_gos_in_field.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "esrc_field.h"
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



// only called in chunkCreate
// all kinds of gos 
void create_gos_in_field(   fieldKey_t      fieldKey_, 
                            const Chunk     &chunkRef_,
                            const Job_Chunk &job_chunkRef_ ){

    auto fieldOpt = esrc::get_fieldPtr(fieldKey_);
    tprAssert( fieldOpt.has_value() );
    const auto &fieldRef = *fieldOpt.value();

    const auto *job_fieldPtr = job_chunkRef_.get_job_fieldPtr(fieldKey_);

    //----- ground go ------//
    if( auto retOpt = job_fieldPtr->get_groundGoDataPtr(); retOpt.has_value() ){
        gameObjs::create_a_Go( retOpt.value() );
    }

    //----- fieldRim go [-DEBUG-] ------//
    //  显示 map 坐标框
    bool isFieldRimGoCreate { false };
    if( isFieldRimGoCreate ){

        auto goDataUPtr = GoDataForCreate::create_new_goDataForCreate(  
                                                    fieldRef.get_midMPos(),
                                                    fieldRef.get_midDPos(),
                                                    GoSpecFromJson::str_2_goSpeciesId("fieldRim"),
                                                    GoAssemblePlanSet::str_2_goLabelId(""),
                                                    NineDirection::Center,
                                                    BrokenLvl::Lvl_0
                                                );
        gameObjs::create_a_Go( goDataUPtr.get() );
    }
    

    //----- bioSoup ------//    
    for( const auto goDataPtr : job_fieldPtr->get_bioSoupGoDataPtrs() ){
        gameObjs::create_a_Go( goDataPtr );
    }

    //----- land majorGo in blueprint -----//
    for( const auto goDataPtr : job_fieldPtr->get_majorGoDataPtrs() ){
        gameObjs::create_a_Go( goDataPtr );
    }

    
    for( const auto goDataPtr : job_fieldPtr->get_floorGoDataPtrs() ){
        gameObjs::create_a_Go( goDataPtr );
    }
    
}


