/*
 * ========================= esrc_ecoSysPlan.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_ECOSYS_PLAN_H_
#define TPR_ESRC_ECOSYS_PLAN_H_

//-------------------- C --------------------//
//#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "EcoSysPlan.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

//inline std::unordered_map<ecoSysPlanId_t, EcoSysPlan> ecoSysPlanes {};

void init_ecoSysPlanes();

EcoSysPlan *get_ecoSysPlanPtr( ecoSysPlanId_t _ecoId );

//-- 一种type，一个子容器，容纳此type 的所有变种 --
//inline std::vector<std::vector<ecoSysPlanId_t>> ecoSysPlanIds_in_type {};

ecoSysPlanId_t apply_a_ecoSysPlanId_by_type( EcoSysPlanType _type, float _ecoObjWeight );

//-- 仅记录所有 ecoId 值 --
//inline std::vector<ecoSysPlanId_t> ecoSysPlanIds {};

ecoSysPlanId_t apply_a_rand_ecoSysPlanId( float _ecoObjWeight );


}//---------------------- namespace: esrc -------------------------//
#endif

