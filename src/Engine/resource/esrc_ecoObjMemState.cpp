/*
 * ================== esrc_ecoObjMemState.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.29
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_ecoObjMemState.h"
//-------------------- CPP --------------------//
#include <deque>

//-------------------- Engine --------------------//
#include "esrc_state.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace ecoObjMS_inn {//------------ namespace: ecoObjMS_inn --------------//

    std::unordered_map<sectionKey_t,EcoObjMemState> ecoObjMemStates {};

    // 注意，当处于 onCreating 时，ecoObj 实例尚未存在于 ecoObjs 中
    std::unordered_set<sectionKey_t>     ecoObjKeys_onCreating {};  //- OnCreating
    std::unordered_set<sectionKey_t>     ecoObjKeys_active {};      //- Active
    std::unordered_set<sectionKey_t>     ecoObjKeys_onReleasing {};    //- OnReleasing


}//---------------- namespace: ecoObjMS_inn end --------------//


void init_ecoObjMemStates()noexcept{
    ecoObjMS_inn::ecoObjMemStates.reserve(100);
    ecoObjMS_inn::ecoObjKeys_onCreating.reserve(100);
    ecoObjMS_inn::ecoObjKeys_active.reserve(100);
    ecoObjMS_inn::ecoObjKeys_onReleasing.reserve(100);
    //---
    esrc::insertState("ecoObjMemState");
}


EcoObjMemState get_ecoObjMemState( sectionKey_t ecoObjKey_ )noexcept{   
    if( ecoObjMS_inn::ecoObjMemStates.find(ecoObjKey_) != ecoObjMS_inn::ecoObjMemStates.end() ){
        return ecoObjMS_inn::ecoObjMemStates.at(ecoObjKey_);
    }else{
        return EcoObjMemState::NotExist;
    }
}


void insert_ecoObjKey_2_onCreating( sectionKey_t ecoObjKey_ )noexcept{
        tprAssert( get_ecoObjMemState(ecoObjKey_) == EcoObjMemState::NotExist ); // MUST
    ecoObjMS_inn::ecoObjKeys_onCreating.insert(ecoObjKey_);
    ecoObjMS_inn::ecoObjMemStates.insert({ ecoObjKey_, EcoObjMemState::OnCreating });
}


void move_ecoObjKey_from_onCreating_2_active( sectionKey_t ecoObjKey_ )noexcept{
        tprAssert( get_ecoObjMemState(ecoObjKey_) == EcoObjMemState::OnCreating ); // MUST
    size_t eraseNum = ecoObjMS_inn::ecoObjKeys_onCreating.erase(ecoObjKey_);
    tprAssert( eraseNum == 1);
    ecoObjMS_inn::ecoObjKeys_active.insert(ecoObjKey_);
    ecoObjMS_inn::ecoObjMemStates.at(ecoObjKey_) = EcoObjMemState::Active;
}


void move_ecoObjKey_from_active_2_onReleasing( sectionKey_t ecoObjKey_ )noexcept{
        tprAssert( get_ecoObjMemState(ecoObjKey_) == EcoObjMemState::Active ); // MUST
    size_t eraseNum = ecoObjMS_inn::ecoObjKeys_active.erase(ecoObjKey_);
    tprAssert( eraseNum == 1);
    ecoObjMS_inn::ecoObjKeys_onReleasing.insert(ecoObjKey_);
    ecoObjMS_inn::ecoObjMemStates.at(ecoObjKey_) = EcoObjMemState::OnReleasing;
}


//- only used by esrc_ecoObj -
void erase_ecoObjKey_from_onReleasing( sectionKey_t ecoObjKey_ )noexcept{
        tprAssert( get_ecoObjMemState(ecoObjKey_) == EcoObjMemState::OnReleasing );
    size_t eraseNum {};
    eraseNum = ecoObjMS_inn::ecoObjKeys_onReleasing.erase(ecoObjKey_);
    tprAssert( eraseNum == 1 );
    eraseNum = ecoObjMS_inn::ecoObjMemStates.erase(ecoObjKey_);
    tprAssert( eraseNum == 1 );
}



}//---------------------- namespace: esrc -------------------------//

