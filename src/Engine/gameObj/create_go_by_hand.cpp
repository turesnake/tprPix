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
#include "create_go_by_hand.h"


//------------------- Engine --------------------//


goid_t create_go_by_hand(   goSpeciesId_t       goSpeciesId_,
                            goLabelId_t         goLabelId_,
                            IntVec2             mpos_,
                            const glm::dvec2    &dpos_,
                            NineDirection       direction_,
                            BrokenLvl           brokenLvl_
                            ){

    auto goDataUPtr = GoDataForCreate::assemble_new_goDataForCreate(  
                                                    mpos_,
                                                    dpos_,
                                                    goSpeciesId_,
                                                    goLabelId_,
                                                    direction_,
                                                    brokenLvl_
                                                );
    //--- dyParam ---//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_Blueprint>();
    fUPtr->goDataPtr = goDataUPtr.get();
    fUPtr->mapEntUWeight = goDataUPtr->uWeight;

    dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
    //---
    goid_t newGoId =  gameObjs::create_a_Go(    goDataUPtr->goSpeciesId,
                                                goDataUPtr->dpos,
                                                dyParam ); 
    return newGoId;
}





// 根据 GoDataForCreate 数据
// 自动创建一个 DyParams_Blueprint
// 然后 新建 go
goid_t create_go_from_goDataForCreate( const GoDataForCreate *goDPtr_ ){

    //--- dyParam ---//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_Blueprint>();
    fUPtr->goDataPtr = goDPtr_;
    fUPtr->mapEntUWeight = goDPtr_->uWeight;

    dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
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
    auto fUPtr = std::make_unique<DyParams_Blueprint>();
    fUPtr->goDataPtr = goDataUPtr.get();
    fUPtr->mapEntUWeight = goDataUPtr->uWeight;

    dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
    //---
    gameObjs::rebind_a_disk_Go( diskGo_.goid,
                                goDataUPtr->goSpeciesId,
                                goDataUPtr->dpos,
                                dyParam ); 
}






