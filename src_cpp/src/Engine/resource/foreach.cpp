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
#include "esrc_ui.h"

//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                     foreach_memGameObjs
 * -----------------------------------------------------------
 * 暂未被使用
 */
/*
void foreach_memGameObjs( F_GOID_GOPTR fp_ ){
    auto ipair = esrc::get_memGameObjs().begin();
    for( ; ipair!=esrc::get_memGameObjs().end(); ipair++ ){
        fp_( ipair->first,  (ipair->second) );
    }
}
*/


/* ===========================================================
 *                     foreach_memUIs
 * -----------------------------------------------------------
 */
void foreach_memUIs( F_UIObjId_GOPTR fp_ ){
    auto ipair = esrc::get_memUIs().begin();
    for( ; ipair!=esrc::get_memUIs().end(); ipair++ ){
        fp_( ipair->first, &(ipair->second) );
    }
}


/* ===========================================================
 *                 foreach_goids_active
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_active( F_GOID_GOPTR fp_ ){

    //GameObj *goPtr {};
    auto it = esrc::get_goids_active().begin();
    for( ; it!=esrc::get_goids_active().end(); it++ ){

        //goPtr = &(esrc::get_memGameObjs().at(*it));
        fp_( *it, esrc::get_goRef(*it) );
    }
}

/* ===========================================================
 *                 foreach_uiIs_active
 * -----------------------------------------------------------
 * -- 
 */
void foreach_uiIds_active( F_UIObjId_GOPTR fp_ ){

    UIObj *uiGoPtr {};
    auto it = esrc::get_uiIds_active().begin();
    for( ; it!=esrc::get_uiIds_active().end(); it++ ){
        uiGoPtr = &(esrc::get_memUIs().at(*it));
        fp_( *it, uiGoPtr );
    }
}



/* ===========================================================
 *                 foreach_goids_inactive
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_inactive( F_GOID_GOPTR fp_ ){

    //GameObj *goPtr {};
    auto it = esrc::get_goids_inactive().begin();
    for( ; it!=esrc::get_goids_inactive().end(); it++ ){

        //goPtr = &(esrc::get_memGameObjs().at(*it));
        fp_( *it, esrc::get_goRef(*it) );
    }
}


}//---------------------- namespace: esrc -------------------------//

