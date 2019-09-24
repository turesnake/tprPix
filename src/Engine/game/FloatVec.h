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


struct FloatVec2{
    float x {0.0f};
    float y {0.0f};
};


inline FloatVec2 glmDvec2_2_floatVec2( const glm::dvec2 dv_ ){
    return FloatVec2{ static_cast<float>(dv_.x), static_cast<float>(dv_.y), };
}


//-- used for ubo --
struct FloatVec4{
    float r {0.0f};
    float g {0.0f};
    float b {0.0f};
    float a {0.0f};
};


#endif 

