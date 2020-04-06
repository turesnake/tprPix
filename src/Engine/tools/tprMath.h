/*
 * ======================== tprMath.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MATH_H
#define TPR_MATH_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"


#ifndef TPR_PI
#define TPR_PI 3.14159
#endif

#ifndef TPR_2PI
#define TPR_2PI 6.28319
#endif


template< typename T >
inline T tprMin( T a_, T b_ )noexcept{
    return ((a_ <= b_) ? a_ : b_);
}

template< typename T >
inline T tprMax( T a_, T b_ )noexcept{
    return ((a_ >= b_) ? a_ : b_);
}

//-- just need overload "<" --
template< typename T >
inline bool is_closeEnough( T a_, T b_, T threshold_ )noexcept{
    return ((a_ < b_) ? ((b_-a_) < threshold_) :
                        ((a_-b_) < threshold_));
}

inline bool is_closeEnough( const glm::dvec2 &a_, const glm::dvec2 &b_, double step_=0.01 )noexcept{
    return (    (std::abs(a_.x - b_.x) <= step_) &&
                (std::abs(a_.y - b_.y) <= step_) );
}





/* ===========================================================
 *                   rotate_vec      
 * -----------------------------------------------------------
 * 将向量 beVec_ 沿0点旋转一个角度， 角度就是 rotateVec_ 与 x轴正方向的夹角
 * 返回旋转后的 向量
 */
/*
inline glm::dvec2 rotate_vec( const glm::dvec2 &beVec_, const glm::dvec2 &rotateVec_ ) noexcept {

        tprAssert( !((rotateVec_.x==0.0) && (rotateVec_.y==0.0)) );
    glm::dvec2 n = glm::normalize( rotateVec_ );
    const glm::dvec2 &t = beVec_;
    //- mutex 
    return glm::dvec2 { (n.x * t.x) - (n.y * t.y),
                        (n.y * t.x) + (n.x * t.y) };
}
*/


/* ===========================================================
 *                  calc_innVec      
 * -----------------------------------------------------------
 * 计算 目标向量 beVec_ 在 基向量 baseVec_ 体内的 向量值
 * 旋转 基向量，使其躺平到 x轴，对齐与 0 点
 * 返回新坐标系中的 目标向量值(向量长度不变)
 */
inline glm::dvec2 calc_innVec( const glm::dvec2 &baseVec_, const glm::dvec2 &beVec_ ) noexcept {

        tprAssert( !((baseVec_.x==0.0) && (baseVec_.y==0.0)) );
    glm::dvec2 n = glm::normalize( baseVec_ );
    const glm::dvec2 &t = beVec_;
    //- mutex 
    return glm::dvec2 { (n.x * t.x) + (n.y * t.y),
                       -(n.y * t.x) + (n.x * t.y) }; //- 注意，要使用反向角度的 矩阵
}




//- 四舍五入
inline double tprRound( double num_ ){
    return (num_>0.0) ? 
        floor( num_ + 0.5 ) :
        ceil( num_ - 0.5 );
}
inline float tprRound( float num_ ){
    return (num_>0.0f) ? 
        floor( num_ + 0.5f ) :
        ceil( num_ - 0.5f );
}
inline glm::dvec2 tprRound( const glm::dvec2 &v_ ){
    return glm::dvec2{  tprRound(v_.x),
                        tprRound(v_.y) };
}


// 基于 uWeight，生成一个 (0.0, 1.0) 的小数随机数
inline double calc_uWeight_fractValue( size_t uWeight ){
    double rd = static_cast<double>(uWeight) / 3.173;
    double integer {}; // 不会被使用
    double fract = modf(rd, &integer); // (0.0, 0.1)
    return fract;
}




#endif 

