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
#include <cmath>

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
        int rr = static_cast<int>(this->r) - static_cast<int>(_a.r);
        int gg = static_cast<int>(this->g) - static_cast<int>(_a.g);
        int bb = static_cast<int>(this->b) - static_cast<int>(_a.b);
        int aa = static_cast<int>(this->a) - static_cast<int>(_a.a);
        return (
        (abs(rr) <= static_cast<int>(_off)) &&
        (abs(gg) <= static_cast<int>(_off)) &&
        (abs(bb) <= static_cast<int>(_off)) &&
        (abs(aa) <= static_cast<int>(_off))
        );
    }

    inline void set( u8_t _r, u8_t _g, u8_t _b, u8_t _a  ){
        this->r = _r;
        this->g = _g; 
        this->b = _b;
        this->a = _a;
    }

    //-- 支持更复杂的数据 累加 --
    inline RGBA add( int _r, int _g, int _b, int _a ) const {
        int rr = static_cast<int>(this->r) + static_cast<int>(_r);
        int gg = static_cast<int>(this->g) + static_cast<int>(_g);
        int bb = static_cast<int>(this->b) + static_cast<int>(_b);
        int aa = static_cast<int>(this->a) + static_cast<int>(_a);
        assert( (rr>=0) && (rr<256) &&
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
    int rr = static_cast<int>(_a.r) + static_cast<int>(_b.r);
    int gg = static_cast<int>(_a.g) + static_cast<int>(_b.g);
    int bb = static_cast<int>(_a.b) + static_cast<int>(_b.b);
    int aa = static_cast<int>(_a.a) + static_cast<int>(_b.a);
        assert( (rr<256) && (gg<256) && (bb<256) && (aa<256) );
    return RGBA {   static_cast<u8_t>(rr), 
                    static_cast<u8_t>(gg),
                    static_cast<u8_t>(bb),
                    static_cast<u8_t>(aa) };
}


namespace rgba {//-------- namespace: rgba --------------//

//-- 只要两个 RGBA 值 足够接近，就算命中 [-常用-] --
inline bool is_rgba_near( const RGBA &_a, const RGBA &_b, u8_t _off ){

    int rr = static_cast<int>(_a.r) - static_cast<int>(_b.r);
    int gg = static_cast<int>(_a.g) - static_cast<int>(_b.g);
    int bb = static_cast<int>(_a.b) - static_cast<int>(_b.b);
    int aa = static_cast<int>(_a.a) - static_cast<int>(_b.a);
    return (
       (std::abs(rr) <= static_cast<int>(_off)) &&
       (std::abs(gg) <= static_cast<int>(_off)) &&
       (std::abs(bb) <= static_cast<int>(_off)) &&
       (std::abs(aa) <= static_cast<int>(_off))
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

    float r = static_cast<float>(_a.r)*_aPercent + static_cast<float>(_b.r)*bPercent;
    float g = static_cast<float>(_a.g)*_aPercent + static_cast<float>(_b.g)*bPercent;
    float b = static_cast<float>(_a.b)*_aPercent + static_cast<float>(_b.b)*bPercent;
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

    float r = static_cast<float>(_a.r) * static_cast<float>(_b.r) / 255.0;
    float g = static_cast<float>(_a.g) * static_cast<float>(_b.g) / 255.0;
    float b = static_cast<float>(_a.b) * static_cast<float>(_b.b) / 255.0;

    return rgba::linear_blend(  _a,
                                RGBA {  static_cast<u8_t>(r), 
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

