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
        gameObjs::create_a_Go(  GoSpecFromJson::str_2_goSpecId( "groundGo" ),
                                    fieldRef.get_midDPos(),
                                    dyParam );
    }


    //----- mapsurface go ------//
    //   直接取消，方便在未来，被全面取代为 蓝图模式
    //--------------------------//



    //----- fieldRim go [-DEBUG-] ------//
    bool isFieldRimGoCreate { false };
    if( isFieldRimGoCreate ){
        gameObjs::create_a_Go(  GoSpecFromJson::str_2_goSpecId( "fieldRim" ),
                                fieldRef.get_midDPos(),
                                emptyDyParam );
    }
    
    //----- land majorGo in old-style -----//
    //  将被 蓝图 彻底 取代
    //--------------------------//
    
   

    //----- land majorGo in blueprint -----//
    //-- 暂时不关 蓝图 和 旧分配方案之间的冲突
    //   在未来，旧方案可能会被彻底 替代

    for( const auto goDataPtr : job_fieldPtr->get_majorGoDataPtrs() ){

        // dir / brokenLvl 这2个数据 暂时未被使用
        // ...

        //--- dyParam ---//
        DyParam dyParam {};
        auto fUPtr = std::make_unique<DyParams_Blueprint>();
        fUPtr->goDataPtr = goDataPtr;
        fUPtr->mapEntUWeight = 12345;   // tmp, 随便写一个值

        dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
        //---

        gameObjs::create_a_Go(  goDataPtr->goSpecId,
                                goDataPtr->dpos,
                                dyParam ); 

    }


    for( const auto goDataPtr : job_fieldPtr->get_floorGoDataPtrs() ){

        // dir / brokenLvl 这2个数据 暂时未被使用
        // ...

        //--- dyParam ---//
        DyParam dyParam {};
        auto fUPtr = std::make_unique<DyParams_Blueprint>();
        fUPtr->goDataPtr = goDataPtr;
        fUPtr->mapEntUWeight = 12345;   // tmp, 随便写一个值

        dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
        //---

        gameObjs::create_a_Go(  goDataPtr->goSpecId,
                                goDataPtr->dpos,
                                dyParam ); 

    }





}


