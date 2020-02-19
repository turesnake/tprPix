/*
 * ======================= esrc_VAOVBO.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   本游戏只有一份 VAO，VBO。
 *   所有的 mesh/goMesh 都应该包含 本文件，直接使用里面的 VAO,VBO
 * ----------------------------
 */
#ifndef TPR_ESRC_VAO_VBO_H
#define TPR_ESRC_VAO_VBO_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h>  

//-------------------- CPP --------------------//
#include <vector>
#include <utility>

//-------------------- Engine --------------------//
#include "VAOVBO.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void init_VAOVBO();
void delete_VAOVBO()noexcept;

GLuint get_VAO()noexcept;
GLsizei get_pointNums()noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif
