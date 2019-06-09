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


#endif
