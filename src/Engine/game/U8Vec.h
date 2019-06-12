/*
 * ========================= U8Vec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    以像素为单位的 vec 向量。
 * ----------------------------
 */
#ifndef TPR_U8_VEC_H_
#define TPR_U8_VEC_H_

//-------------------- C --------------------//
#include <cmath>


//------------------- Libs --------------------//
#include "tprDataType.h"


//-- 少数场合会用到的 vec2 ----
// 不推荐大规模使用
class U8Vec2{
public:
    U8Vec2() = default;
    U8Vec2( u8_t _x, u8_t _y ):
        x(_x),
        y(_y)
        {}

    inline void clear_all(){
        this->x = 0;
        this->y = 0;
    }

    inline void set( u8_t _x, u8_t _y ){
        this->x = _x;
        this->y = _y;
    }

    //======== vals ========//
    u8_t  x {0};
    u8_t  y {0};
};


#endif 

