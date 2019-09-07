/*
 * ========================= U8Vec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    以像素为单位的 vec 向量。
 * ----------------------------
 */
#ifndef TPR_U8_VEC_H
#define TPR_U8_VEC_H

//-------------------- C --------------------//
#include <cmath>


//------------------- Libs --------------------//
#include "tprDataType.h"


//-- 少数场合会用到的 vec2 ----
// 不推荐大规模使用
// 希望简化类型种类，尽可能统一到 int,double 中去
class U8Vec2{
public:
    U8Vec2() = default;
    U8Vec2( u8_t x_, u8_t y_ ):
        x(x_),
        y(y_)
        {}

    inline void clear_all() noexcept {
        this->x = 0;
        this->y = 0;
    }

    inline void set( u8_t x_, u8_t y_ ) noexcept {
        this->x = x_;
        this->y = y_;
    }

    //======== vals ========//
    u8_t  x {0};
    u8_t  y {0};
};


#endif 

