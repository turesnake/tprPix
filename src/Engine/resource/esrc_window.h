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


//-- 一个 类实例 维护一个 window。
//-- window 数据结构的 具体内容由 glfw库管理。
//-- 用户只需保存一个指针。用来访问这个 window。
//inline GLFWwindow  *windowPtr {};

GLFWwindow *get_windowPtr();

void set_windowPtr( GLFWwindow *_newPtr );


}//---------------------- namespace: esrc -------------------------//
#endif

