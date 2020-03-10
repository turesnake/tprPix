/*
 * ===================== esrc_ecoSysPlan.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  ecoSysPlanes
 * ----------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "random.h"
#include "esrc_ecoSysPlan.h"
#include "esrc_gameSeed.h"

//-------------------- Script --------------------//
#include "Script/json/json_all.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace ecoPlan_inn {//-------- namespace: ecoPlan_inn --------------//
    
    std::unordered_map<ecoSysPlanId_t, std::unique_ptr<EcoSysPlan>> ecoSysPlanes {};

    //-- 一种type，一个子容器，容纳此type 的所有变种 --
    std::vector<std::vector<ecoSysPlanId_t>> ecoSysPlanIds_in_type {};

    //-- 仅记录所有 ecoId 值 --
    std::vector<ecoSysPlanId_t> ecoSysPlanIds {};
    

}//------------- namespace: ecoPlan_inn end --------------//


void init_ecoSysPlanes(){

    ecoPlan_inn::ecoSysPlanIds_in_type.resize( EcoSysPlanType_Num );
    //---------------------//
    json::parse_ecoSysPlansJsonFile();
    
    //---------------------//
    //   shuffle ecoSysPlanIds
    //---------------------//
    auto &shuffleEngine = esrc::get_gameSeed().getnc_shuffleEngine(); 
    std::shuffle(   ecoPlan_inn::ecoSysPlanIds.begin(), 
                    ecoPlan_inn::ecoSysPlanIds.end(),
                    shuffleEngine );
}



EcoSysPlan *get_ecoSysPlanPtr( ecoSysPlanId_t ecoId_ ){
        tprAssert( ecoPlan_inn::ecoSysPlanes.find(ecoId_) != ecoPlan_inn::ecoSysPlanes.end() ); //- tmp
    return ecoPlan_inn::ecoSysPlanes.at(ecoId_).get();
}



EcoSysPlan &insert_new_ecoSysPlan( EcoSysPlanType type_ ){

    ecoSysPlanId_t ecoPlanId = EcoSysPlan::id_manager.apply_a_u32_id();
    auto ecoPlanUPtr = std::make_unique<EcoSysPlan>();
    ecoPlanUPtr->set_id( ecoPlanId );
    ecoPlanUPtr->set_type( type_ );

    auto [insertIt, insertBool] = ecoPlan_inn::ecoSysPlanes.emplace( ecoPlanId, std::move(ecoPlanUPtr) );
    tprAssert( insertBool );//- must not exist

    ecoPlan_inn::ecoSysPlanIds_in_type.at(ecoSysPlanType_2_idx(type_)).push_back(ecoPlanId);
    ecoPlan_inn::ecoSysPlanIds.push_back(ecoPlanId);

    return *(insertIt->second);
}


/* ===========================================================
 *             apply_a_ecoSysPlanId_by_type
 * -----------------------------------------------------------
 * -- 指定了 type，在此基础上，分配一个 变种id
 */
ecoSysPlanId_t apply_a_ecoSysPlanId_by_type( EcoSysPlanType type_, size_t ecoObjUWeight_ ){
    auto &container = ecoPlan_inn::ecoSysPlanIds_in_type.at(ecoSysPlanType_2_idx(type_));    
    return container.at( (ecoObjUWeight_ + 707013) % container.size() );
}


/* ===========================================================
 *              apply_a_rand_ecoSysPlanId
 * -----------------------------------------------------------
 */
ecoSysPlanId_t apply_a_rand_ecoSysPlanId( size_t ecoObjUWeight_ ){
    size_t idx = (ecoObjUWeight_ + 337791) % ecoPlan_inn::ecoSysPlanIds.size();
    return ecoPlan_inn::ecoSysPlanIds.at(idx);
}






}//---------------------- namespace: esrc -------------------------//
