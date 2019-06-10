/*
 * ========================= esrc_ecoSysPlan.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_ECOSYS_PLAN_H_
#define TPR_ESRC_ECOSYS_PLAN_H_

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "EcoSysPlan.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void init_ecoSysPlanes();

EcoSysPlan *get_ecoSysPlanPtr( ecoSysPlanId_t _ecoId );

ecoSysPlanId_t apply_a_ecoSysPlanId_by_type( EcoSysPlanType _type, float _ecoObjWeight );

ecoSysPlanId_t apply_a_rand_ecoSysPlanId( float _ecoObjWeight );


}//---------------------- namespace: esrc -------------------------//
#endif

