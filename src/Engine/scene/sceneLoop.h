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
    //FirstPlayInputSet, //- 第一次启动游戏时，强制进入 按键设置界面
    Begin, //- 最最基础的 游戏启动界面
    
    World  //- 主游戏
};


inline F_void sceneRenderLoopFunc {nullptr};
inline F_void sceneLogicLoopFunc {nullptr};


//void prepareForScene_firstPlayInputSet();
void prepareForScene_begin();
void prepareForScene_world();


void switch_sceneLoop( SceneLoopType type_ );


#endif 

