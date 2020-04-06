/*
 * =================== UIGoSpecFromJson.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "UIGoSpecFromJson.h"


//-------------------- Engine --------------------//
#include "GameObj.h"


//======== static ========//
std::unordered_map<goSpeciesId_t, std::unique_ptr<UIGoSpecFromJson>> UIGoSpecFromJson::dataUPtrs {};
std::unordered_map<goSpeciesId_t, std::string>     UIGoSpecFromJson::ids_2_names {};
std::unordered_map<std::string, goSpeciesId_t>     UIGoSpecFromJson::names_2_ids {};
std::unordered_map<goSpeciesId_t, F_GO_INIT>       UIGoSpecFromJson::initFuncs {};




void UIGoSpecFromJson::assemble_2_newUIGo( goSpeciesId_t specID_, GameObj &goRef_ ){

    const auto &d = UIGoSpecFromJson::get_uiGoSpecFromJsonRef( specID_ );


    goRef_.speciesId = d.speciesId;
    goRef_.family    = d.family;

    goRef_.move.set_MoveType( d.moveType );
    goRef_.move.moveSpeedLvl.reset( d.moveSpeedLvl );
   

    // 对于 uiGo 来说，很多 go成员 都不需要
    // 直接使用 默认值...

    // 有些则需要手动设置
    //...
    goRef_.isMoveCollide = false;

    //------ default -------//
    goRef_.isDirty = false;         
    goRef_.isControlByPlayer = false;
    //...
    //------ tmp -------//
    goRef_.isActive = true; //- tmp. 是否进入激活圈，应该由 mpos 计算出来 未实现  
    //...
}





