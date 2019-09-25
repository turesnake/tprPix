/*
 * ========================= foreach.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   所有的 foreach_ 系列函数
 * ----------------------------
 */
#include "esrc_gameObj.h"

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                 foreach_goids_active
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_active( F_GOID_GOPTR fp_ ){

    auto it = esrc::get_goids_active().begin();
    for( ; it!=esrc::get_goids_active().end(); it++ ){
        fp_( *it, esrc::get_goRef(*it) );
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
        fp_( *it, esrc::get_goRef(*it) );
    }
}


}//---------------------- namespace: esrc -------------------------//

