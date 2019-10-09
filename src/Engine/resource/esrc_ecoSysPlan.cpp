/*
 * ===================== esrc_ecoSysPlan.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  ecoSysPlanes
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <memory>
//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "random.h"
#include "esrc_ecoSysPlan.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc_all.h" 
#include "Script/json/json_all.h"

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace ecoPlan_inn {//-------- namespace: ecoPlan_inn --------------//
    
    std::unordered_map<ecoSysPlanId_t, std::unique_ptr<EcoSysPlan>> ecoSysPlanes {};

    //-- 一种type，一个子容器，容纳此type 的所有变种 --
    std::vector<std::vector<ecoSysPlanId_t>> ecoSysPlanIds_in_type {};

    //-- 仅记录所有 ecoId 值 --
    std::vector<ecoSysPlanId_t> ecoSysPlanIds {};
    

}//------------- namespace: ecoPlan_inn end --------------//


EcoSysPlan *get_ecoSysPlanPtr( ecoSysPlanId_t ecoId_ ){
        tprAssert( ecoPlan_inn::ecoSysPlanes.find(ecoId_) != ecoPlan_inn::ecoSysPlanes.end() ); //- tmp
    return ecoPlan_inn::ecoSysPlanes.at(ecoId_).get();
}



EcoSysPlan &insert_new_ecoSysPlan( EcoSysPlanType type_ ){

    ecoSysPlanId_t ecoPlanId = EcoSysPlan::id_manager.apply_a_u32_id();
    auto ecoPlanUPtr = std::make_unique<EcoSysPlan>();
    ecoPlanUPtr->set_id( ecoPlanId );
    ecoPlanUPtr->set_type( type_ );

    auto outPair = ecoPlan_inn::ecoSysPlanes.insert({ ecoPlanId, std::move(ecoPlanUPtr) });
    tprAssert( outPair.second );//- must not exist

    ecoPlan_inn::ecoSysPlanIds_in_type.at(ecoSysPlanType_2_idx(type_)).push_back(ecoPlanId);
    ecoPlan_inn::ecoSysPlanIds.push_back(ecoPlanId);

    return *(outPair.first->second);
}


/* ===========================================================
 *             apply_a_ecoSysPlanId_by_type
 * -----------------------------------------------------------
 * -- 指定了 type，在此基础上，分配一个 变种id
 */
ecoSysPlanId_t apply_a_ecoSysPlanId_by_type( EcoSysPlanType type_, size_t ecoObjUWeight_ ){
    auto &container = ecoPlan_inn::ecoSysPlanIds_in_type.at(ecoSysPlanType_2_idx(type_));    
    return container.at( (ecoObjUWeight_ * 3 + 707013) % container.size() );
}


/* ===========================================================
 *              apply_a_rand_ecoSysPlanId
 * -----------------------------------------------------------
 */
ecoSysPlanId_t apply_a_rand_ecoSysPlanId( size_t ecoObjUWeight_ ){
    size_t idx = (ecoObjUWeight_ + 337791) % ecoPlan_inn::ecoSysPlanIds.size();
    return ecoPlan_inn::ecoSysPlanIds.at(idx);
}


/* ===========================================================
 *                      init_ecoSysPlanes
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一初始化 所有 EcoSysPlan 资源
 *    不追求性能
 */
void init_ecoSysPlanes(){

    ecoPlan_inn::ecoSysPlanes.clear();
    ecoPlan_inn::ecoSysPlanIds_in_type.clear();
    ecoPlan_inn::ecoSysPlanIds_in_type.resize( EcoSysPlanType_Num );
    ecoPlan_inn::ecoSysPlanIds.clear();
    //---------------------//
    
    //ecoPlan_inn::init_Forest_1();
    //ecoPlan_inn::init_Forest_2();
    //ecoPlan_inn::init_DarkForest_1();
    //ecoPlan_inn::init_Plain_1();
    //ecoPlan_inn::init_Swamp_1();
    //ecoPlan_inn::init_Desert_1();
    //ecoPlan_inn::init_Desert_2();

    json::parse_from_ecoSysPlansJsonFile();
    
    //---------------------//
    //   shuffle ecoSysPlanIds
    //---------------------//
    std::default_random_engine randEngine; 
    randEngine.seed(431); //- 提供固定seed
    std::shuffle(   ecoPlan_inn::ecoSysPlanIds.begin(), 
                    ecoPlan_inn::ecoSysPlanIds.end(),
                    randEngine );
}


}//---------------------- namespace: esrc -------------------------//
