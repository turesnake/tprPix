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
void switch_sceneLoop( SceneLoopType type_ ){

    switch (type_){
    case SceneLoopType::Begin:
        sceneLogicLoopFunc = std::bind( sceneLogicLoop_begin );
        sceneRenderLoopFunc = std::bind( sceneRenderLoop_begin );
        break;

    case SceneLoopType::World:
        sceneLogicLoopFunc = std::bind( sceneLogicLoop_world );
        sceneRenderLoopFunc = std::bind( sceneRenderLoop_world );
        break;
    
    default:
        tprAssert(0);
        break;
    }

}

