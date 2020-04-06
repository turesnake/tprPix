/*
 * ========================= IntVec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    以像素为单位的 vec 向量。
 * ----------------------------
 */
#ifndef TPR_PIX_VEC_H
#define TPR_PIX_VEC_H

//-------------------- CPP --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprCast.h"

//--- [mem] --//
class IntVec2{
public:
    IntVec2() = default;
    IntVec2( int x_, int y_ ):
        x(x_),
        y(y_)
        {}
    
    IntVec2( size_t x_, size_t y_ ):
        x(static_cast<int>(x_)),
        y(static_cast<int>(y_))
        {}

    inline void clear_all() noexcept {
        this->x = 0;
        this->y = 0;
    }

    //--------
    inline IntVec2& operator += ( IntVec2 a_ ) noexcept {
        this->x += a_.x;
        this->y += a_.y;
        return *this;
    }
    inline IntVec2& operator -= ( IntVec2 a_ ) noexcept {
        this->x -= a_.x;
        this->y -= a_.y;
        return *this;
    }
    inline IntVec2& operator *= ( int m_ ) noexcept {
        this->x *= m_;
        this->y *= m_;
        return *this;
    }

    //-- 地板除法，向低取节点值 --
    //  -1- double 除法
    //  -2- math.floor()
    inline IntVec2 floorDiv( double div_ ) const  noexcept {
        double fx = static_cast<double>(this->x) / div_;
        double fy = static_cast<double>(this->y) / div_;
        return IntVec2{ static_cast<int>(floor(fx)),
                        static_cast<int>(floor(fy)) };
    }

    //======== static ========//
    static bool is_closeEnough( IntVec2 v1_, IntVec2 v2_, size_t off_ ) noexcept;

    //======== vals ========//
    int  x {0}; 
    int  y {0};
};


//======= static =======//
// x/y 差值均小于 off_ 时，返回 true 
inline bool IntVec2::is_closeEnough( IntVec2 v1_, IntVec2 v2_, size_t off_ ) noexcept {
    size_t off_x = cast_2_size_t( std::abs( static_cast<double>(v1_.x-v2_.x) ) );
    size_t off_y = cast_2_size_t( std::abs( static_cast<double>(v1_.y-v2_.y) ) ); 
                                            // prevent std::abs ambiguous
    return ( (off_x<off_) && (off_y<off_) );
}


// std::hash 特化
// IntVec2 可以成为 std::unordered_map / std::unordered_set 的 key
namespace std{
template<> 
struct hash<IntVec2>{
    hash()=default;
    size_t operator()(IntVec2 iv_) const{
        size_t key {};
        int *ptr = (int*)(&key); //- can't use static_cast<>
        *ptr = iv_.x;
        ptr++;
        *ptr = iv_.y; 
        //--------
        return key;
    }
};
}// namespace std



/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline bool operator == ( IntVec2 a_, IntVec2 b_ ) noexcept {
    return ( (a_.x==b_.x) && (a_.y==b_.y) );
}
inline bool operator != ( IntVec2 a_, IntVec2 b_ ) noexcept {
    return ( (a_.x!=b_.x) || (a_.y!=b_.y) );
}

/* ===========================================================
 *                   operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，IntVec2 类型将支持 set.find() 
 * -- IntVec2 可以成为 std::map / std::set 的 key
 */
inline bool operator < ( IntVec2 a_, IntVec2 b_ ) noexcept {
    if( a_.x == b_.x ){
        return ( a_.y < b_.y );
    }
    return ( a_.x < b_.x );
}

/* ===========================================================
 *                   operator +, -
 * -----------------------------------------------------------
 */
inline IntVec2 operator + ( IntVec2 a_, IntVec2 b_ ) noexcept {
    return IntVec2 { a_.x+b_.x, a_.y+b_.y };
}
inline IntVec2 operator - ( IntVec2 a_, IntVec2 b_ ) noexcept {
    return IntVec2 { a_.x-b_.x, a_.y-b_.y };
}

/* ===========================================================
 *                   operator * 
 * -----------------------------------------------------------
 */
inline IntVec2 operator * ( IntVec2 a_, int m_ ) noexcept {
    return IntVec2 { a_.x*m_, a_.y*m_ };
}
inline IntVec2 operator * ( int m_, IntVec2 a_ ) noexcept {
    return IntVec2 { a_.x*m_, a_.y*m_ };
}







/* ===========================================================
 *                     floorDiv
 * -----------------------------------------------------------
 * -- 地板除运算 [通过 "double地板除"，解决了负数bug] --
 *  -1- double 除法
 *  -2- math.floor()
 */
inline IntVec2 floorDiv(  IntVec2 a_, double div_ ) noexcept {
    double fx = static_cast<double>(a_.x) / div_;
    double fy = static_cast<double>(a_.y) / div_;
    return IntVec2{ static_cast<int>(floor(fx)),
                    static_cast<int>(floor(fy)) };
}


/* ===========================================================
 *                       floorMod
 * -----------------------------------------------------------
 * -- 取模运算 [通过 "double地板除"， 解决了负数bug] --
 *  -1- double 除法
 *  -2- math.floor()
 */
inline IntVec2 floorMod( IntVec2 v_, double mod_ ) noexcept {
    double fx = ( static_cast<double>(v_.x) ) / mod_;
    double fy = ( static_cast<double>(v_.y) ) / mod_;
    double floorX = floor(fx) * mod_;
    double floorY = floor(fy) * mod_;
    return IntVec2{ v_.x - static_cast<int>(floor(floorX)),
                    v_.y - static_cast<int>(floor(floorY)) };
}



#endif

