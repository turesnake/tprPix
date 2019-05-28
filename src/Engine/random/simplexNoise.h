/*
 * ========================= simplexNoise.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.04.02
 *                                        修改 -- 
 * ----------------------------------------------------------
 *   glsl版 simplex-noise, 移植到 cpu上
 * ----------------------------
 */
#ifndef _TPR_SIMPLEX_NOISE_H_
#define _TPR_SIMPLEX_NOISE_H_

//--- glm - 0.9.9.5 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

float simplex_noise2( const glm::vec2 &v );
float simplex_noise2( float _x, float _y );



#endif 

