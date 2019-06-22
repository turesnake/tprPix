/*
 * ========================= GameObjINS.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   go所接受不了的 游戏指令组 
 *  -----
 *  物理鼠键 -> inputINS -> GameObjINS
 * ----------------------------
 */
#ifndef TPR_GAME_OBJ_INS_H_
#define TPR_GAME_OBJ_INS_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- Libs --------------------//
#include "tprDataType.h" 


class GameObjINS{
public:

    //======== vals ========//
    //  确保对齐，提高传输效率
    //  尽可能缩小尺寸。确保 值传递也能高效
    glm::vec2 dir   { 0.0f, 0.0f }; //-  go 单位方向向量(暂由鼠标实现)
    
    char      crossX   {0};    //- 十字键 [-1,0,1]
    char      crossY   {0};
};


#endif 

