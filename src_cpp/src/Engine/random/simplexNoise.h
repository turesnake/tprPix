/*
 * ========================= simplexNoise.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.04.02
 *                                        修改 -- 
 * ----------------------------------------------------------
 *   glsl版 simplex-noise, 移植到 cpu上
 * ----------------------------
 */
#ifndef TPR_SIMPLEX_NOISE_H
#define TPR_SIMPLEX_NOISE_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

float simplex_noise2( const glm::vec2 &v_ );
float simplex_noise2( float x_, float y_ );



#endif 

