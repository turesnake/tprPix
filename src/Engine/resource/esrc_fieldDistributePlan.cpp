/*
 * ================= esrc_fieldDistributePlan.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_fieldDistributePlan.h"


//-------------------- CPP --------------------//
#include <memory>

//-------------------- Engine --------------------//
#include "esrc_state.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace fdp_inn {//-------- namespace: fdp_inn --------------//
 
    std::unique_ptr<FieldDistributePlanSet> fieldDistributePlanSetUPtr {nullptr};

}//------------- namespace: fdp_inn end --------------//


void init_fieldDistributePlanSet()noexcept{
    fdp_inn::fieldDistributePlanSetUPtr = std::make_unique<FieldDistributePlanSet>();
    esrc::insertState("fieldDistributePlanSet");
}



FieldDistributePlanSet &get_fieldDistributePlanSet()noexcept{
    tprAssert( fdp_inn::fieldDistributePlanSetUPtr );
    return *(fdp_inn::fieldDistributePlanSetUPtr);
}



}//---------------------- namespace: esrc -------------------------//











