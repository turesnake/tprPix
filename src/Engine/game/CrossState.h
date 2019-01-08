/*
 * ========================= CrossState.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    基于十字键的  位移运动状态
 * ----------------------------
 */
#ifndef _TPR_CROSS_STATE_H_
#define _TPR_CROSS_STATE_H_

//- 十字键 x／y轴 状态：
//  -1 -- 左／下
//   0 -- 无
//   1 -- 右／上
struct CrossState{
    int  x  {0};
    int  y  {0};
};


enum class DIRECTION{
    Idle, //- 无方向
    Left,
    Right,
    Up,
    Down
};


#endif 

