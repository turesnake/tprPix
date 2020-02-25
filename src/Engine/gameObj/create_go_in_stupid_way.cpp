/*
 * ================= create_go_in_stupid_way.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 *  临时版，使用类似 蓝图的流程，在主线程中，手动创建一个 go
 * 
 * -----
 */
#include "create_go_in_stupid_way.h"


//------------------- Engine --------------------//


goid_t create_go_in_stupid_way( goSpeciesId_t       goSpeciesId_,
                                goLabelId_t         goLabelId_,
                                const glm::dvec2    &dpos_,
                                NineDirection       direction_,
                                BrokenLvl           brokenLvl_,
                                size_t              mapEntUWeight_ ){

    auto goDataUPtr = GoDataForCreate::assemble_new_goDataForCreate(  
                                                    dpos_,
                                                    goSpeciesId_,
                                                    goLabelId_,
                                                    direction_,
                                                    brokenLvl_,
                                                    mapEntUWeight_
                                                );
    //--- dyParam ---//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_Blueprint>();
    fUPtr->goDataPtr = goDataUPtr.get();
    fUPtr->mapEntUWeight = mapEntUWeight_;

    dyParam.insert_ptr<DyParams_Blueprint>( fUPtr.get() );
    //---
    goid_t newGoId =  gameObjs::create_a_Go(    goDataUPtr->goSpeciesId,
                                                goDataUPtr->dpos,
                                                dyParam ); 
    return newGoId;
}















