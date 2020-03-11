/*
 * ========================= foreach.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   所有的 foreach_ 系列函数
 * ----------------------------
 */
#include "pch.h"
#include "esrc_gameObj.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                 foreach_goids_active
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_active( F_GOID_GOPTR fp_ ){

    auto it = esrc::get_goids_active().begin();
    for( ; it!=esrc::get_goids_active().end(); it++ ){

        auto goOpt = esrc::get_goPtr( *it );
        tprAssert( goOpt.has_value() );
        GameObj &goRef = *goOpt.value();
        //--
        fp_( *it, goRef );
    }
}


/* ===========================================================
 *                 foreach_goids_inactive
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_inactive( F_GOID_GOPTR fp_ ){

    auto it = esrc::get_goids_inactive().begin();
    for( ; it!=esrc::get_goids_inactive().end(); it++ ){

        auto goOpt = esrc::get_goPtr( *it );
        tprAssert( goOpt.has_value() );
        GameObj &goRef = *goOpt.value();
        //--
        fp_( *it, goRef );
    }
}


}//---------------------- namespace: esrc -------------------------//

