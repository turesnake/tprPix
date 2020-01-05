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
std::unordered_map<goSpeciesId_t, std::unique_ptr<GoSpecFromJson>> GoSpecFromJson::dataUPtrs {};
std::unordered_map<goSpeciesId_t, std::string>     GoSpecFromJson::ids_2_names {};
std::unordered_map<std::string, goSpeciesId_t>     GoSpecFromJson::names_2_ids {};
std::unordered_map<goSpeciesId_t, F_GO_INIT>       GoSpecFromJson::initFuncs {}; 



void GoSpecFromJson::assemble_2_newGo( goSpeciesId_t specID_, GameObj &goRef_ ){

    const auto &d = GoSpecFromJson::get_goSpecFromJsonRef( specID_ );

    goRef_.speciesId = d.speciesId;
    goRef_.family    = d.family;
    goRef_.state     = d.state;
    goRef_.moveState = d.moveState;
    goRef_.move.set_MoveType( d.moveType );

    goRef_.isMoveCollide = d.isMoveCollide;

    auto &collisionRef = goRef_.get_collisionRef();

    collisionRef.set_isDoPass( d.isDoPass );
    collisionRef.set_isBePass( d.isBePass );
    if( !d.isBePass ){
        collisionRef.set_functor_Is_extraPassableDogoSpeciesId( 
            [&d](goSpeciesId_t dogoSpeciesId_)->bool {
                return (d.extraPassableDogoSpeciesIds.find(dogoSpeciesId_) != d.extraPassableDogoSpeciesIds.end());
            }
        );
    }

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




void GoSpecFromJson::convert_all_extraPassableDogoSpeciesNames_2_goSpeciesIds()noexcept{

    for( const auto &ipair : GoSpecFromJson::dataUPtrs ){ // each goSpecFromJson
        GoSpecFromJson &goSpecFromJsonRef = *(ipair.second); 

        if( goSpecFromJsonRef.isBePass ||
            goSpecFromJsonRef.extraPassableDogoSpeciesNames.empty() ){
            continue; // skip
        }

        //---
        for( const auto &name : goSpecFromJsonRef.extraPassableDogoSpeciesNames ){ // each name
            goSpecFromJsonRef.extraPassableDogoSpeciesIds.insert( GoSpecFromJson::str_2_goSpeciesId(name) );
        }
        goSpecFromJsonRef.extraPassableDogoSpeciesNames.clear(); // no need
    }
}



