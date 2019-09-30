/*
 * ========================= FloatVec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.22
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_FLOAT_VEC_H
#define TPR_FLOAT_VEC_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "tprMath.h"


struct FloatVec2{
    float x {0.0f};
    float y {0.0f};
};


inline FloatVec2 glmDvec2_2_floatVec2( const glm::dvec2 dv_ ){
    return FloatVec2{ static_cast<float>(dv_.x), static_cast<float>(dv_.y), };
}




//-- used for ubo --
struct FloatVec3{
    float x {0.0f};
    float y {0.0f};
    float z {0.0f};

    //--- operator += ... ---//
    inline FloatVec3& operator += ( const FloatVec3 &a_ ) noexcept {
        this->x += a_.x;
        this->y += a_.y;
        this->z += a_.z;
        return *this;
    }
    inline FloatVec3& operator *= ( float s_ ) noexcept {
        this->x *= s_;
        this->y *= s_;
        this->z *= s_;
        return *this;
    }
};




//-- used for ubo --
struct FloatVec4{
    float r {0.0f};
    float g {0.0f};
    float b {0.0f};
    float a {0.0f};

    //--- operator += ... ---//
    inline FloatVec4& operator += ( const FloatVec4 &a_ ) noexcept {
        this->r += a_.r;
        this->g += a_.g;
        this->b += a_.b;
        this->a += a_.a;
        return *this;
    }
};

/* ===========================================================
 *                   operator +, -, *
 * -----------------------------------------------------------
 */
inline FloatVec4 operator + ( const FloatVec4 &a_, const FloatVec4 &b_ )noexcept{
    return FloatVec4 {  a_.r + b_.r, 
                        a_.g + b_.g,
                        a_.b + b_.b,
                        a_.a + b_.a };
}
inline FloatVec4 operator - ( const FloatVec4 &a_, const FloatVec4 &b_ )noexcept{
    return FloatVec4 {  a_.r - b_.r, 
                        a_.g - b_.g,
                        a_.b - b_.b,
                        a_.a - b_.a };
}
inline FloatVec4 operator * ( const FloatVec4 &a_, float s_ )noexcept{
    return FloatVec4 {  a_.r * s_, 
                        a_.g * s_,
                        a_.b * s_,
                        a_.a * s_ };
}


//-- include alpha channel --
inline bool is_closeEnough( const FloatVec4 &a_, const FloatVec4 &b_, float threshold_ )noexcept{
    return (is_closeEnough<float>(a_.r, b_.r, threshold_) &&
            is_closeEnough<float>(a_.g, b_.g, threshold_) &&
            is_closeEnough<float>(a_.b, b_.b, threshold_) &&
            is_closeEnough<float>(a_.a, b_.a, threshold_) );
}

inline bool is_closeEnough_without_alpha( const FloatVec4 &a_, const FloatVec4 &b_, float threshold_ )noexcept{
    return (is_closeEnough<float>(a_.r, b_.r, threshold_) &&
            is_closeEnough<float>(a_.g, b_.g, threshold_) &&
            is_closeEnough<float>(a_.b, b_.b, threshold_) );
}





#endif 

