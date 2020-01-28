/*
 * ===================== glm_no_warnings.h =================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GLM_NO_WARNINGS_H_
#define TPR_GLM_NO_WARNINGS_H_

//-- 屏蔽掉 glm 中的所有 warnings --
#pragma clang system_header

//--- glm - 0.9.9.5 ---
#include <glm_inn/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm_inn/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm_inn/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- C --------------------//
#include <cmath>

inline glm::dvec2 glm_vec2_2_dvec2( const glm::vec2 &fv_ ){
    return glm::dvec2{ static_cast<double>(fv_.x),
                        static_cast<double>(fv_.y) };
}
inline glm::vec2 glm_dvec2_2_vec2( const glm::dvec2 &dv_ ){
    return glm::vec2{ static_cast<float>(dv_.x),
                        static_cast<float>(dv_.y) };
}

inline glm::dvec3 glm_vec3_2_dvec3( const glm::vec3 &fv_ ){
    return glm::dvec3{ static_cast<double>(fv_.x),
                        static_cast<double>(fv_.y),
                        static_cast<double>(fv_.z) };
}
inline glm::vec3 glm_dvec3_2_vec3( const glm::dvec3 &dv_ ){
    return glm::vec3{ static_cast<float>(dv_.x),
                        static_cast<float>(dv_.y),
                        static_cast<float>(dv_.z) };
}

#endif
