/*
 * ========================= PixVec.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.28
 *                                        修改 -- 2018.11.28
 * ----------------------------------------------------------
 *    以像素为单位的 vec 向量。
 * ----------------------------
 */
#ifndef _TPR_PIX_VEC2_H_
#define _TPR_PIX_VEC2_H_

//-------------------- C --------------------//
#include <cassert>


struct PixVec2{
    int  x {0}; 
    int  y {0};
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( const PixVec2 &_a, const PixVec2 &_b ){
    return ( (_a.x==_b.x) && (_a.y==_b.y) );
}
inline bool operator != ( const PixVec2 &_a, const PixVec2 &_b ){
    return ( (_a.x!=_b.x) || (_a.y!=_b.y) );
}

/* ===========================================================
 *                   operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，PixVec2 类型将支持 set.find() 
 */
inline bool operator < ( const PixVec2 &_a, const PixVec2 &_b ){
    if( _a.x == _b.x ){
        return ( _a.y < _b.y );
    }
    return ( _a.x < _b.x );
}

/* ===========================================================
 *                   operator +, -
 * -----------------------------------------------------------
 */
inline PixVec2 operator + ( const PixVec2 &_a, const PixVec2 &_b ){
    return PixVec2 { _a.x+_b.x, _a.y+_b.y };
}
inline PixVec2 operator - ( const PixVec2 &_a, const PixVec2 &_b ){
    return PixVec2 { _a.x-_b.x, _a.y-_b.y };
}

/* ===========================================================
 *                   operator * 
 * -----------------------------------------------------------
 */
inline PixVec2 operator * ( const PixVec2 &_a, int _m ){
    return PixVec2 { _a.x*_m, _a.y*_m };
}
inline PixVec2 operator * ( int _m, const PixVec2 &_a ){
    return PixVec2 { _a.x*_m, _a.y*_m };
}

/* ===========================================================
 *                   operator /
 * -----------------------------------------------------------
 */
inline PixVec2 operator / ( const PixVec2 &_a, int _div ){
    assert( _div != 0 );
    return PixVec2 { _a.x/_div, _a.y/_div };
}




struct PixVec3{
    int  x  {0}; 
    int  y  {0};
    int  z  {0};
};
inline bool is_equal( const PixVec3 &_a, const PixVec3 &_b ){
    return ( ((_a.x==_b.x)&&(_a.y==_b.y)&&(_a.z==_b.z)) );
}




#endif

