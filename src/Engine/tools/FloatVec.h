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
#include "RGBA.h"


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
    inline FloatVec3& operator += ( float f_ ) noexcept {
        this->x += f_;
        this->y += f_;
        this->z += f_;
        return *this;
    }
    inline FloatVec3& operator *= ( float s_ ) noexcept {
        this->x *= s_;
        this->y *= s_;
        this->z *= s_;
        return *this;
    }
};

inline FloatVec3 operator + ( FloatVec3 a_, FloatVec3 b_ ) noexcept {
    return FloatVec3 { a_.x+b_.x, a_.y+b_.y, a_.z+b_.z };
}
inline FloatVec3 operator - ( FloatVec3 a_, FloatVec3 b_ ) noexcept {
    return FloatVec3 { a_.x-b_.x, a_.y-b_.y, a_.z-b_.z };
}
inline FloatVec3 operator + ( FloatVec3 a_, float f_ ) noexcept {
    return FloatVec3 { a_.x+f_, a_.y+f_, a_.z+f_ };
}
inline FloatVec3 operator - ( FloatVec3 a_, float f_ ) noexcept {
    return FloatVec3 { a_.x-f_, a_.y-f_, a_.z-f_ };
}
inline FloatVec3 operator * ( FloatVec3 a_, float f_ ) noexcept {
    return FloatVec3 { a_.x*f_, a_.y*f_, a_.z*f_ };
}



inline bool is_closeEnough( const FloatVec3 &a_, const FloatVec3 &b_, float threshold_ )noexcept{
    return (is_closeEnough<float>(a_.x, b_.x, threshold_) &&
            is_closeEnough<float>(a_.y, b_.y, threshold_) &&
            is_closeEnough<float>(a_.z, b_.z, threshold_) );
}


inline FloatVec3 rgba_2_floatVec3( const RGBA &rgba_ )noexcept{
    float r = static_cast<float>(rgba_.r) / 255.0f; // [0.0, 1.0]
    float g = static_cast<float>(rgba_.g) / 255.0f; // [0.0, 1.0]
    float b = static_cast<float>(rgba_.b) / 255.0f; // [0.0, 1.0]
    // ignore alpha
    return FloatVec3{ r, g, b };
}





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


inline FloatVec4 floatVec3_2_floatVec4( const FloatVec3 &v_, float alpha_ )noexcept{
    return FloatVec4{ v_.x, v_.y, v_.z, alpha_ };
}
inline FloatVec3 floatVec4_2_floatVec3( const FloatVec4 &v_ )noexcept{
    return FloatVec3{ v_.r, v_.g, v_.b };
}




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

