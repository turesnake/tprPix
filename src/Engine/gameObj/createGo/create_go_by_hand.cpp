/*
 * ================= create_go_by_hand.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 *  临时版，使用类似 蓝图的流程，在主线程中，手动创建一个 go
 * 
 * -----
 */
#include "create_goes.h"

//------------------- Engine --------------------//
#include "DiskGameObj.h"


namespace gameObjs{//------------- namespace gameObjs ----------------




// 根据 GoDataForCreate 数据
// 自动创建一个 DyParams_GoDataForCreate
// 然后 新建 go
goid_t create_go_from_goDataForCreate( const GoDataForCreate *goDPtr_ ){

    //--- dyParam ---//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_GoDataForCreate>();
    fUPtr->goDataPtr = goDPtr_;
    //fUPtr->mapEntUWeight = goDPtr_->uWeight;

    dyParam.insert_ptr<DyParams_GoDataForCreate>( fUPtr.get() );
    //---
    goid_t newGoId = gameObjs::create_a_Go( goDPtr_->goSpeciesId,
                                            goDPtr_->dpos,
                                            dyParam ); 
    return newGoId;
}





void rebind_diskGo_by_hand( const DiskGameObj &diskGo_ ){

    auto goDataUPtr = GoDataForCreate::assemble_new_goDataForCreate(  
                                                    dpos_2_mpos(diskGo_.dpos),
                                                    diskGo_.dpos,
                                                    diskGo_.goSpeciesId,
                                                    diskGo_.goLabelId,
                                                    diskGo_.dir,
                                                    diskGo_.brokenLvl
                                                );
    //--- dyParam ---//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_GoDataForCreate>();
    fUPtr->goDataPtr = goDataUPtr.get();
    //fUPtr->mapEntUWeight = goDataUPtr->uWeight;

    dyParam.insert_ptr<DyParams_GoDataForCreate>( fUPtr.get() );
    //---
    gameObjs::rebind_a_disk_Go( diskGo_.goid,
                                goDataUPtr->goSpeciesId,
                                goDataUPtr->dpos,
                                dyParam ); 
}





}//------------- namespace gameObjs: end ----------------

