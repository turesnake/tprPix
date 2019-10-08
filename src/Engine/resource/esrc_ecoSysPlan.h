/*
 * ========================= esrc_ecoSysPlan.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_ECOSYS_PLAN_H
#define TPR_ESRC_ECOSYS_PLAN_H

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "EcoSysPlan.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void init_ecoSysPlanes();

EcoSysPlan &insert_new_ecoSysPlan( EcoSysPlanType type_ );

EcoSysPlan *get_ecoSysPlanPtr( ecoSysPlanId_t ecoId_ );

ecoSysPlanId_t apply_a_ecoSysPlanId_by_type( EcoSysPlanType type_, size_t ecoObjUWeight_ );

ecoSysPlanId_t apply_a_rand_ecoSysPlanId( size_t ecoObjUWeight_ );


}//---------------------- namespace: esrc -------------------------//
#endif

