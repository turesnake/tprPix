/*
 * ====================== GameSeed.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   game seed for random
 * ----------------------------
 */
#ifndef _TPR_GameSeed_H_
#define _TPR_GameSeed_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "random.h"


//- singleton --
// 一个存档只有一个 gameSeed 实例 --
// 具体的 随机引擎和分布器，则由各模块自行包含 （tmp）
class GameSeed{
public:

    void init();



    //======== vals ========//
    u32_t  baseSeed {}; //-- 最基础的那颗种子，其它种子由它生成。









};





#endif 

