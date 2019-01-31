/*
 * ========================= RGBA.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   png pix color: RGBA
 * ----------------------------
 */
#ifndef _TPR_RGBA_H_
#define _TPR_RGBA_H_

//------------------- C --------------------//
#include <math.h>

//------------------- Libs --------------------//
#include "tprDataType.h"


enum class RGBA_ChannelType : u8 {
    R = 1,
    G,
    B,
    A
};


//-- 1个 png 像素 的 data 结构 --
class RGBA{
public:
    u8 r {0};
    u8 g {0};
    u8 b {0};
    u8 a {255};
    //----------

    inline bool is_near( const RGBA &_a, u8 _off ){
        return (
        (abs(r-_a.r) <= _off) &&
        (abs(g-_a.g) <= _off) &&
        (abs(b-_a.b) <= _off) &&
        (abs(a-_a.a) <= _off)
        );
    }

};

//-- 只要两个 RGBA 值 足够接近，就算命中 [-常用-] --
/*
inline bool is_near( const RGBA &_a, const RGBA &_b, u8 _off ){
    return (
       (abs(_a.r-_b.r) <= _off) &&
       (abs(_a.g-_b.g) <= _off) &&
       (abs(_a.b-_b.b) <= _off) &&
       (abs(_a.a-_b.a) <= _off)
    );
}
*/

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator==( const RGBA &_a, const RGBA &_b  ){
    return ( (_a.r==_b.r) && (_a.g==_b.g) && (_a.b==_b.b) && (_a.a==_b.a) );
}
inline bool operator!=( const RGBA &_a, const RGBA &_b  ){
    return ( (_a.r!=_b.r) || (_a.g!=_b.g) || (_a.b!=_b.b) || (_a.a!=_b.a) );
}




#endif

