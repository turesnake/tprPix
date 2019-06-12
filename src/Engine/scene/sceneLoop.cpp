/*
 * ==================== sceneLoop.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "sceneLoop.h"
#include "sceneLoopInn.h"


/* ===========================================================
 *                   switch_sceneLoop
 * -----------------------------------------------------------
 */
void switch_sceneLoop( const SceneLoopType &_type ){

    switch (_type){
    case SceneLoopType::Begin:
        sceneLoopFunc = std::bind( sceneLoop_begin );
        break;

    case SceneLoopType::World:
        sceneLoopFunc = std::bind( sceneLoop_world );
        break;
    
    default:
        tprAssert(0);
        break;
    }

}

