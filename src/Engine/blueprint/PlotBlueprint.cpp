/*
 * ======================= PlotBlueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "PlotBlueprint.h"

//-------------------- Engine --------------------//
#include "esrc_gameSeed.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//

//============== static ===============//
ID_Manager                                          PlotBlueprint::id_manager { ID_TYPE::U32, 0};
std::unordered_map<std::string, std::unordered_map<std::string, plotBlueprintId_t>> PlotBlueprint::name_2_ids {};
std::unordered_map<plotBlueprintId_t, std::unique_ptr<PlotBlueprint>> PlotBlueprint::plotUPtrs {};
//---
ID_Manager          VarTypeDatas_Plot::id_manager { ID_TYPE::U32, 0 };


// [*main-thread*]
void PlotBlueprint::init_for_static()noexcept{
    PlotBlueprint::name_2_ids.reserve(1000);
    PlotBlueprint::plotUPtrs.reserve(1000);
}



void VarTypeDatas_Plot::init_check()noexcept{
    tprAssert( !this->goSpecPool.empty() );
    tprAssert( !this->randPool.empty() );
    //-- shuffle --//
    auto &shuffleEngine = esrc::get_gameSeed().getnc_shuffleEngine(); 
    std::shuffle( this->randPool.begin(), this->randPool.end(), shuffleEngine );
}


/* [static]
 * 外部禁止 自行创建 Plot 实例，必须通过此函数
 */
plotBlueprintId_t PlotBlueprint::init_new_plot( const std::string &plotName_, const std::string &plotLabel_ ){
    //-- name_2_ids
    plotBlueprintId_t id {};

    if( plotName_ == "_PLACE_HOLDER_" ){
        tprAssert( plotLabel_ == "_PLACE_HOLDER_" );
        id = PlotBlueprint::placeHolderId;
    }else{
        id = PlotBlueprint::id_manager.apply_a_u32_id();
        if( PlotBlueprint::is_placeHolderId(id) ){
            id = PlotBlueprint::id_manager.apply_a_u32_id(); // apply again
        }
    }
    //---
    auto [insertIt1, insertBool1] = PlotBlueprint::name_2_ids.insert({ plotName_, std::unordered_map<std::string, plotBlueprintId_t>{} }); // maybe
    auto &innUMap = insertIt1->second;
    
    
    auto [insertIt2, insertBool2] = innUMap.insert({ plotLabel_, id });
    tprAssert( insertBool2 );
    

    //-- plotUPtrs 
    std::unique_ptr<PlotBlueprint> uptr ( new PlotBlueprint() ); // can't use std::make_unique
    auto [insertIt3, insertBool3] = PlotBlueprint::plotUPtrs.insert({ id, std::move(uptr) });
    tprAssert( insertBool3 );
    
    //--
    return id;
}


}//--------------------- namespace: blueprint end ------------------------//

