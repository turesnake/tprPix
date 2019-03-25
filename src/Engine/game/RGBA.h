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
#include <cassert>
#include <math.h>

//------------------- Libs --------------------//
#include "tprDataType.h"


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
    RGBA( u8_t _r, u8_t _g, u8_t _b, u8_t _a ):
        r(_r),
        g(_g),
        b(_b),
        a(_a)
        {}
    
    inline bool is_near( const RGBA &_a, u8_t _off )const{

        int rr = (int)r - (int)_a.r;
        int gg = (int)g - (int)_a.g;
        int bb = (int)b - (int)_a.b;
        int aa = (int)a - (int)_a.a;
        return (
        (abs(rr) <= _off) &&
        (abs(gg) <= _off) &&
        (abs(bb) <= _off) &&
        (abs(aa) <= _off)
        );
    }

    inline void set( u8_t _r, u8_t _g, u8_t _b, u8_t _a  ){
        r = _r;
        g = _g; 
        b = _b;
        a = _a;
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
inline bool operator==( const RGBA &_a, const RGBA &_b  ){
    return ( (_a.r==_b.r) && (_a.g==_b.g) && (_a.b==_b.b) && (_a.a==_b.a) );
}
inline bool operator!=( const RGBA &_a, const RGBA &_b  ){
    return ( (_a.r!=_b.r) || (_a.g!=_b.g) || (_a.b!=_b.b) || (_a.a!=_b.a) );
}


/* ===========================================================
 *                   operator +
 * -----------------------------------------------------------
 */
inline RGBA operator + ( const RGBA &_a, const RGBA &_b ){
    int r = (int)(_a.r) + (int)(_b.r);
    int g = (int)(_a.g) + (int)(_b.g);
    int b = (int)(_a.b) + (int)(_b.b);
    int a = (int)(_a.a) + (int)(_b.a);
        assert( (r<256) && (g<256) && (b<256) && (a<256) );
    return RGBA {   static_cast<u8_t>(_a.r+_b.r), 
                    static_cast<u8_t>(_a.g+_b.g),
                    static_cast<u8_t>(_a.b+_b.b),
                    static_cast<u8_t>(_a.a+_b.a) };
}


namespace rgba {//-------- namespace: rgba --------------//

//-- 只要两个 RGBA 值 足够接近，就算命中 [-常用-] --
inline bool is_rgba_near( const RGBA &_a, const RGBA &_b, u8_t _off ){

    int rr = (int)_a.r - (int)_b.r;
    int gg = (int)_a.g - (int)_b.g;
    int bb = (int)_a.b - (int)_b.b;
    int aa = (int)_a.a - (int)_b.a;
    return (
       (abs(rr) <= _off) &&
       (abs(gg) <= _off) &&
       (abs(bb) <= _off) &&
       (abs(aa) <= _off)
    );
}


/* ===========================================================
 *                   linear_blend
 * -----------------------------------------------------------
 * 将两个颜色 线性混合
 * param: _aPercent -- 颜色 _a 占了多少百分比 [0.0, 1.0]
 */
inline RGBA linear_blend( const RGBA &_a, const RGBA &_b, float _aPercent ){

    assert( (_aPercent>=0.0) && (_aPercent<=1.0) );

    float bPercent = 1.0 - _aPercent;

    float r = (float)(_a.r)*_aPercent + (float)(_b.r)*bPercent;
    float g = (float)(_a.g)*_aPercent + (float)(_b.g)*bPercent;
    float b = (float)(_a.b)*_aPercent + (float)(_b.b)*bPercent;
    //-- 默认不处理 RGBA.a --

    return RGBA {   static_cast<u8_t>(r), 
                    static_cast<u8_t>(g),
                    static_cast<u8_t>(b),
                    255 };
}


/* ===========================================================
 *                   multiply
 * -----------------------------------------------------------
 *  简易版 正片叠底
 *  假设 _a.a 永远等于 255， 通过 参数 _bPercent，来调节 正片叠底 程度
 * param: _bPercent -- 正片叠底 的 程度 [0.0, 1.0]
 */
inline RGBA multiply( const RGBA &_a, const RGBA &_b, float _bPercent ){

    assert( (_bPercent>=0.0) && (_bPercent<=1.0) );

    float r = (float)(_a.r) * (float)(_b.r) / 255.0;
    float g = (float)(_a.g) * (float)(_b.g) / 255.0;
    float b = (float)(_a.b) * (float)(_b.b) / 255.0;

    return rgba::linear_blend(  _a,
                                RGBA {   static_cast<u8_t>(r), 
                                        static_cast<u8_t>(g),
                                        static_cast<u8_t>(b),
                                        255 },
                                (1.0-_bPercent) );
    /*
    return RGBA {   static_cast<u8_t>(r), 
                    static_cast<u8_t>(g),
                    static_cast<u8_t>(b),
                    255 };
    */
}


}//------------- namespace: rgba end --------------//
#endif

