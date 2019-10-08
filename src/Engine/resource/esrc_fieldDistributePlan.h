/*
 * ================= esrc_fieldDistributePlan.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_FIELD_DISTRIBUTE_PLAN_H
#define TPR_ESRC_FIELD_DISTRIBUTE_PLAN_H

//-------------------- Engine --------------------//
#include "FieldDistributePlan.h" 

namespace esrc {//------------------ namespace: esrc -------------------------//  


void init_fieldDistributePlanSet()noexcept;
FieldDistributePlanSet &get_fieldDistributePlanSet()noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif

