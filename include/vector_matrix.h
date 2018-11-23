/*
 * ========================= vector_matrix.h ==========================
 *                          -- GL_CPP --
 *                                        创建 -- 2018.08.27
 *                                        修改 -- 2018.08.27
 * ----------------------------------------------------------
 *   基于 glm， 与 向量／矩阵 有关的 一些 独立函数
 * ----------------------------
 */

#ifndef _TPR_VECTOR_MATRIX_H_
#define _TPR_VECTOR_MATRIX_H_


//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//----------------------


//-- 提供一个 单位矩阵。
//-- 此矩阵 永远不会被改写。专供 中间计算
//-- 默认初始化为 单位矩阵。
inline glm::mat4 normal_mat4_ {}; 

//-- 提供一个 零向量。
//-- 此向量 永远不会被改写。专供 中间计算
//-- 默认初始化为 0向量
inline glm::vec3 vec3_zero_ {};


//-- 提供一个 3元素 皆为 1.0f 的向量常量。
inline glm::vec3 vec3_one_ { 1.0f, 1.0f, 1.0f };




#endif
