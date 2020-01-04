/*
 * =================== GoSpecFromJson.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoSpecFromJson.h"


//-------------------- Engine --------------------//
#include "GameObj.h"


//======== static ========//
std::unordered_map<goSpecId_t, std::unique_ptr<GoSpecFromJson>> GoSpecFromJson::dataUPtrs {};
std::unordered_map<goSpecId_t, std::string>     GoSpecFromJson::ids_2_names {};
std::unordered_map<std::string, goSpecId_t>     GoSpecFromJson::names_2_ids {};
std::unordered_map<goSpecId_t, F_GO_INIT>       GoSpecFromJson::initFuncs {}; 



void GoSpecFromJson::assemble_2_newGo( goSpecId_t specID_, GameObj &goRef_ ){

    const auto &d = GoSpecFromJson::get_goSpecFromJsonRef( specID_ );

    goRef_.species   = d.specID;
    goRef_.family    = d.family;
    goRef_.state     = d.state;
    goRef_.moveState = d.moveState;
    goRef_.move.set_MoveType( d.moveType );

    goRef_.isMoveCollide = d.isMoveCollide;
    goRef_.get_collisionRef().set_isDoPass( d.isDoPass );
    goRef_.get_collisionRef().set_isBePass( d.isBePass );

    goRef_.move.set_speedLvl( d.speedLvl );
    goRef_.set_pos_alti( d.alti );
    goRef_.weight = d.weight;

    //------ pubBinary -------//
    goRef_.pubBinary.HP = d.pubBinary.HP;
    goRef_.pubBinary.MP = d.pubBinary.MP;

    //...
    //------ default -------//
    goRef_.isDirty = false;
    goRef_.isControlByPlayer = false;
    //...
    //------ tmp -------//
    goRef_.isActive = true; //- tmp. 是否进入激活圈，应该由 mpos 计算出来 未实现
    //...
}






