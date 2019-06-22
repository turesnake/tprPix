/*
 * ========================= esrc_window.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_WINDOW_H_
#define TPR_ESRC_WINDOW_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- Engine --------------------//
#include "IntVec.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


GLFWwindow *get_windowPtr();

void set_windowPtr( GLFWwindow *_newPtr );


}//---------------------- namespace: esrc -------------------------//
#endif

