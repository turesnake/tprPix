/*
 * ========================= ActionSwitchType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    这个文件 应该放到 script 层
 * ----------------------------
 */
#ifndef TPR_ACTION_SWITCH_TYPE_H
#define TPR_ACTION_SWITCH_TYPE_H

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-- 如果 元素数量超过 64个，就需要修改 相关内容
//  千万不能 自定义 元素的值。应该让元素的值按照持续，被自动分配(从0开始增长)
//  元素值将被转换为 idx，用来访问 bitMap
enum class ActionSwitchType : u32_t{

    //--- move ----//
    Move_Idle, //- 移动中途 停留 时的动画（树木的待机也算）
    Move_Move, //- 移动时的动画

    BeCollide_From_Left,  //- 受到来自左侧的碰撞后，播放的动画
    BeCollide_From_Right  //- 受到来自右侧的碰撞后，播放的动画
    //...
};




#endif 

