/*
 * ========================= esrc_shader.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_SHADER_H_
#define TPR_ESRC_SHADER_H_

//-------------------- Engine --------------------//
#include "ShaderProgram.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

ShaderProgram &get_rect_shader();

void init_shaders();

}//---------------------- namespace: esrc -------------------------//
#endif

