/*
 * ====================== sceneLoop.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SCENE_LOOP_H
#define TPR_SCENE_LOOP_H

//-------------------- CPP --------------------//
#include <functional> 

//-------------------- Engine --------------------//
#include "functorTypes.h"


enum class SceneLoopType : int{
    Null,
    Begin, //- 最最基础的 游戏启动界面
    
    World  //- 主游戏
};


inline F_void sceneRenderLoopFunc {nullptr};
inline F_void sceneLogicLoopFunc {nullptr};


void prepare_for_sceneBegin();
void prepare_for_sceneWorld();

void switch_sceneLoop( SceneLoopType type_ );


#endif 

