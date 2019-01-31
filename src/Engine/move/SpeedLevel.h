/*
 * ========================= SpeedLevel.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动: 爬行
 * ----------------------------
 */
#ifndef _TPR_SPEED_LEVEL_H_
#define _TPR_SPEED_LEVEL_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-- go 可设置的 8档 速度 --
enum class SpeedLevel : u8 {
    LV_1 = 1, //- 低速
    LV_2 = 2, 
    LV_3 = 3, 
    LV_4 = 4, 
    LV_5 = 5, 
    LV_6 = 6, 
    LV_7 = 7, 
    LV_8 = 8  //- 高速
};


#endif 

