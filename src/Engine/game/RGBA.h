/*
 * ========================= RGBA.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.26
 *                                        修改 -- 2018.12.26
 * ----------------------------------------------------------
 *   png pix color: RGBA
 * ----------------------------
 */
#ifndef _TPR_RGBA_H_
#define _TPR_RGBA_H_

//------------------- Libs --------------------//
#include "tprDataType.h"

//-- 1个 png 像素 的 data 结构 --
struct RGBA{
    u8 r {0};
    u8 g {0};
    u8 b {0};
    u8 a {0};
};
inline bool is_equal( const RGBA &_a, const RGBA &_b ){
    return ( (_a.r==_b.r)&&(_a.g==_b.g)&&(_a.b==_b.b)&&(_a.a==_b.a) );
}

#endif

