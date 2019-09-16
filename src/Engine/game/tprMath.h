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


#include <cmath>


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


inline bool is_closeEnough( double a_, double b_, double threshold_=0.01 ) noexcept {
    return ( std::abs(a_-b_) < threshold_ );
}


/* ===========================================================
 *                   rotate_vec      
 * -----------------------------------------------------------
 * 将向量 beVec_ 沿0点旋转一个角度， 角度就是 rotateVec_ 与 x轴正方向的夹角
 * 返回旋转后的 向量
 */
inline glm::dvec2 rotate_vec( const glm::dvec2 &beVec_, const glm::dvec2 &rotateVec_ ) noexcept {

        tprAssert( !((rotateVec_.x==0.0) && (rotateVec_.y==0.0)) );
    glm::dvec2 n = glm::normalize( rotateVec_ );
    const glm::dvec2 &t = beVec_;
    //- mutex 
    return glm::dvec2 { (n.x * t.x) - (n.y * t.y),
                        (n.y * t.x) + (n.x * t.y) };
}








#endif 

