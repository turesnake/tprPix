/*
 * ========================= esrc_window.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_WINDOW_H
#define TPR_ESRC_WINDOW_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- Engine --------------------//
#include "IntVec.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


GLFWwindow *get_windowPtr();

void set_windowPtr( GLFWwindow *newPtr_ );


}//---------------------- namespace: esrc -------------------------//
#endif

