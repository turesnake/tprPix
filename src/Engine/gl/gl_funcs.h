/*
 * ========================= gl_funcs.h ===================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   gl 模块中的 公共函数
 * ----------------------------
 */
#ifndef _TPR_GL_H_
#define _TPR_GL_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//------------------- Engine --------------------//
#include "PixVec.h"

//------ gl_support.cpp -----//
void glfw_init();
void glfw_hints_set();
void glfw_window_creat();
void glfw_oth_set();
void glfw_callback_set();

void glad_init();
void glad_set();

#endif
