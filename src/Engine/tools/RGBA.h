/*
 * ========================= RGBA.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   png pix color: RGBA
 * ----------------------------
 */
#ifndef TPR_RGBA_H
#define TPR_RGBA_H
//------------------- C --------------------//
#include <cmath>

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "tprAssert.h"


enum class RGBA_ChannelType : u8_t {
    R = 1,
    G,
    B,
    A
};


//-- 1个 png 像素 的 data 结构 --
class RGBA{
public: 
    //---- constructor -----//
    RGBA() = default;
    RGBA( u8_t r_, u8_t g_, u8_t b_, u8_t a_ ):
        r(r_),
        g(g_),
        b(b_),
        a(a_)
        {}
    
    inline bool is_near( const RGBA &a_, u8_t off_ ) const noexcept {
        int rr = static_cast<int>(this->r) - static_cast<int>(a_.r);
        int gg = static_cast<int>(this->g) - static_cast<int>(a_.g);
        int bb = static_cast<int>(this->b) - static_cast<int>(a_.b);
        int aa = static_cast<int>(this->a) - static_cast<int>(a_.a);
        return (
        (std::abs(rr) <= static_cast<int>(off_)) &&
        (std::abs(gg) <= static_cast<int>(off_)) &&
        (std::abs(bb) <= static_cast<int>(off_)) &&
        (std::abs(aa) <= static_cast<int>(off_))
        );
    }

    inline void set( u8_t r_, u8_t g_, u8_t b_, u8_t a_  ) noexcept {
        this->r = r_;
        this->g = g_; 
        this->b = b_;
        this->a = a_;
    }

    //-- 支持更复杂的数据 累加 --
    inline RGBA add( int r_, int g_, int b_, int a_ ) const noexcept {
        int rr = static_cast<int>(this->r) + r_;
        int gg = static_cast<int>(this->g) + g_;
        int bb = static_cast<int>(this->b) + b_;
        int aa = static_cast<int>(this->a) + a_;
        tprAssert(  (rr>=0) && (rr<256) &&
                    (gg>=0) && (gg<256) &&
                    (bb>=0) && (bb<256) );
        return RGBA {   static_cast<u8_t>(rr), 
                        static_cast<u8_t>(gg), 
                        static_cast<u8_t>(bb), 
                        static_cast<u8_t>(aa) };
    }


    //======== vals ========//
    u8_t r {0};
    u8_t g {0};
    u8_t b {0};
    u8_t a {255};
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator==( const RGBA &a_, const RGBA &b_  ) noexcept {
    return ( (a_.r==b_.r) && (a_.g==b_.g) && (a_.b==b_.b) && (a_.a==b_.a) );
}
inline bool operator!=( const RGBA &a_, const RGBA &b_  ) noexcept {
    return ( (a_.r!=b_.r) || (a_.g!=b_.g) || (a_.b!=b_.b) || (a_.a!=b_.a) );
}


/* ===========================================================
 *                   operator +
 * -----------------------------------------------------------
 */
inline RGBA operator + ( const RGBA &a_, const RGBA &b_ ) noexcept {
    int rr = static_cast<int>(a_.r) + static_cast<int>(b_.r);
    int gg = static_cast<int>(a_.g) + static_cast<int>(b_.g);
    int bb = static_cast<int>(a_.b) + static_cast<int>(b_.b);
    int aa = static_cast<int>(a_.a) + static_cast<int>(b_.a);
        tprAssert( (rr<256) && (gg<256) && (bb<256) && (aa<256) );
    return RGBA {   static_cast<u8_t>(rr), 
                    static_cast<u8_t>(gg),
                    static_cast<u8_t>(bb),
                    static_cast<u8_t>(aa) };
}


class HSV{
public:
    double h {}; // [0.0,360.0] degree
    double s {}; // [0.0,1.0] pct
    double v {}; // [0.0,1.0] pct
};


/* 计算结果精度有限：
 *   -- h    存在 +- 2.0 以内的误差，当明度较低时，误差会变大
 *   -- s/v  存在 +- 0.2 以内的误差
 */
inline HSV rgb_2_hsv( RGBA in_){

    double ir = static_cast<double>(in_.r) / 255.0; // [0.0, 1.0]
    double ig = static_cast<double>(in_.g) / 255.0; // [0.0, 1.0]
    double ib = static_cast<double>(in_.b) / 255.0; // [0.0, 1.0]
    // ignore alpha 

    HSV         out {};
    double      min {}; 
    double      max {};
    double      delta {};
    //---
    min = ir < ig ? ir : ig;
    min = min  < ib ? min  : ib;

    max = ir > ig ? ir : ig;
    max = max  > ib ? max  : ib;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0.0;
        out.h = 0.0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( ir >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( ig - ib ) / delta;        // between yellow & magenta
    else
    if( ig >= max )
        out.h = 2.0 + ( ib - ir ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( ir - ig ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


namespace rgba {//-------- namespace: rgba --------------//

//-- 只要两个 RGBA 值 足够接近，就算命中 [-常用-] --
inline bool is_rgba_near( const RGBA &a_, const RGBA &b_, u8_t off_ ) noexcept {

    int rr = static_cast<int>(a_.r) - static_cast<int>(b_.r);
    int gg = static_cast<int>(a_.g) - static_cast<int>(b_.g);
    int bb = static_cast<int>(a_.b) - static_cast<int>(b_.b);
    int aa = static_cast<int>(a_.a) - static_cast<int>(b_.a);
    return (
       (std::abs(rr) <= static_cast<int>(off_)) &&
       (std::abs(gg) <= static_cast<int>(off_)) &&
       (std::abs(bb) <= static_cast<int>(off_)) &&
       (std::abs(aa) <= static_cast<int>(off_))
    );
}


/* ===========================================================
 *                   linear_blend
 * -----------------------------------------------------------
 * 将两个颜色 线性混合
 * param: aPercent_ -- 颜色 a_ 占了多少百分比 [0.0, 1.0]
 */
inline RGBA linear_blend( const RGBA &a_, const RGBA &b_, double aPercent_ ) noexcept {

    tprAssert( (aPercent_>=0.0) && (aPercent_<=1.0) );

    double bPercent = 1.0 - aPercent_;

    double r = static_cast<double>(a_.r)*aPercent_ + static_cast<double>(b_.r)*bPercent;
    double g = static_cast<double>(a_.g)*aPercent_ + static_cast<double>(b_.g)*bPercent;
    double b = static_cast<double>(a_.b)*aPercent_ + static_cast<double>(b_.b)*bPercent;
    //-- 默认不处理 RGBA.a --

    return RGBA {   static_cast<u8_t>(r), 
                    static_cast<u8_t>(g),
                    static_cast<u8_t>(b),
                    255 };
}


/* 简易版 正片叠底
 *  假设 a_.a 永远等于 255， 通过 参数 _bPercent，来调节 正片叠底 程度
 * param: bPercent_ -- 正片叠底 的 程度 [0.0, 1.0]
 */
inline RGBA multiply( const RGBA &a_, const RGBA &b_, double bPercent_ ) noexcept {

    tprAssert( (bPercent_>=0.0) && (bPercent_<=1.0) );

    double r = static_cast<double>(a_.r) * static_cast<double>(b_.r) / 255.0;
    double g = static_cast<double>(a_.g) * static_cast<double>(b_.g) / 255.0;
    double b = static_cast<double>(a_.b) * static_cast<double>(b_.b) / 255.0;

    return rgba::linear_blend(  a_,
                                RGBA {  static_cast<u8_t>(r), 
                                        static_cast<u8_t>(g),
                                        static_cast<u8_t>(b),
                                        255 },
                                (1.0-bPercent_) );
}


}//------------- namespace: rgba end --------------//
#endif

