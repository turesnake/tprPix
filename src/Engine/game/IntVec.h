/*
 * ========================= IntVec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    以像素为单位的 vec 向量。
 * ----------------------------
 */
#ifndef _TPR_PIX_VEC_H_
#define _TPR_PIX_VEC_H_

//-------------------- C --------------------//
#include <cassert>
#include <math.h>


//--- [mem] --//
class IntVec2{
public:
    IntVec2() = default;
    IntVec2( int _x, int _y ):
        x(_x),
        y(_y)
        {}

    inline void clear_all(){
        x = 0;
        y = 0;
    }

    inline void set( int _x, int _y ){
        x = _x;
        y = _y;
    }

    //--------
    inline IntVec2& operator += ( const IntVec2 &_a ){
        x += _a.x;
        y += _a.y;
        return *this;
    }
    inline IntVec2& operator -= ( const IntVec2 &_a ){
        x -= _a.x;
        y -= _a.y;
        return *this;
    }
    inline IntVec2& operator *= ( int _m ){
        x *= _m;
        y *= _m;
        return *this;
    }

    //-- 地板除法，向低取节点值 --
    //  -1- float 除法
    //  -2- math.floor()
    inline IntVec2 floorDiv( float _div ){
        float fx = ((float)x) / _div;
        float fy = ((float)y) / _div;
        return IntVec2{ (int)floor(fx),
                        (int)floor(fy) };
    }

    //======== vals ========//
    int  x {0}; 
    int  y {0};
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( const IntVec2 &_a, const IntVec2 &_b ){
    return ( (_a.x==_b.x) && (_a.y==_b.y) );
}
inline bool operator != ( const IntVec2 &_a, const IntVec2 &_b ){
    return ( (_a.x!=_b.x) || (_a.y!=_b.y) );
}

/* ===========================================================
 *                   operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，IntVec2 类型将支持 set.find() 
 */
inline bool operator < ( const IntVec2 &_a, const IntVec2 &_b ){
    if( _a.x == _b.x ){
        return ( _a.y < _b.y );
    }
    return ( _a.x < _b.x );
}

/* ===========================================================
 *                   operator +, -
 * -----------------------------------------------------------
 */
inline IntVec2 operator + ( const IntVec2 &_a, const IntVec2 &_b ){
    return IntVec2 { _a.x+_b.x, _a.y+_b.y };
}
inline IntVec2 operator - ( const IntVec2 &_a, const IntVec2 &_b ){
    return IntVec2 { _a.x-_b.x, _a.y-_b.y };
}

/* ===========================================================
 *                   operator * 
 * -----------------------------------------------------------
 */
inline IntVec2 operator * ( const IntVec2 &_a, int _m ){
    return IntVec2 { _a.x*_m, _a.y*_m };
}
inline IntVec2 operator * ( int _m, const IntVec2 &_a ){
    return IntVec2 { _a.x*_m, _a.y*_m };
}


/* ===========================================================
 *                     floorDiv
 * -----------------------------------------------------------
 * -- 地板除运算 [通过 "float地板除"，解决了负数bug] --
 *  -1- float 除法
 *  -2- math.floor()
 */
inline IntVec2 floorDiv(  const IntVec2 &_a, float _div ){
    float fx = ((float)_a.x) / _div;
    float fy = ((float)_a.y) / _div;
    return IntVec2{ (int)floor(fx),
                    (int)floor(fy) };
}


/* ===========================================================
 *                       floorMod
 * -----------------------------------------------------------
 * -- 取模运算 [通过 "float地板除"， 解决了负数bug] --
 *  -1- float 除法
 *  -2- math.floor()
 */
inline IntVec2 floorMod( const IntVec2 &_v, float _mod ){
    float fx = ((float)_v.x) / _mod;
    float fy = ((float)_v.y) / _mod;
    float floorX = floor(fx) * _mod;
    float floorY = floor(fy) * _mod;
    return IntVec2{ _v.x - (int)floorX,
                    _v.y - (int)floorY };
}



//--- [mem] --//
class IntVec3{
public:
    int  x  {0}; 
    int  y  {0};
    int  z  {0};
};
inline bool is_equal( const IntVec3 &_a, const IntVec3 &_b ){
    return ( ((_a.x==_b.x)&&(_a.y==_b.y)&&(_a.z==_b.z)) );
}




#endif

