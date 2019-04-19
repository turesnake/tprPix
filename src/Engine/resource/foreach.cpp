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

//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                     foreach_memGameObjs
 * -----------------------------------------------------------
 * -- 
 */
void foreach_memGameObjs( F_GOID_GOPTR _fp ){

    auto ipair = esrc::memGameObjs.begin();
    for( ; ipair!=esrc::memGameObjs.end(); ipair++ ){
        _fp( ipair->first, &(ipair->second) );
    }
}


/* ===========================================================
 *                 foreach_goids_active
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_active( F_GOID_GOPTR _fp ){

    GameObj *goPtr;
    auto it = esrc::goids_active.begin();
    for( ; it!=esrc::goids_active.end(); it++ ){

        goPtr = static_cast<GameObj*>( &(esrc::memGameObjs.at(*it)) );
        _fp( *it, goPtr );
    }
}



/* ===========================================================
 *                 foreach_goids_inactive
 * -----------------------------------------------------------
 * -- 
 */
void foreach_goids_inactive( F_GOID_GOPTR _fp ){

    GameObj *goPtr;
    auto it = esrc::goids_inactive.begin();
    for( ; it!=esrc::goids_inactive.end(); it++ ){

        goPtr = static_cast<GameObj*>( &(esrc::memGameObjs.at(*it)) );
        _fp( *it, goPtr );
    }
}


}//---------------------- namespace: esrc -------------------------//

