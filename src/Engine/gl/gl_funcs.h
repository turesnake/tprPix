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
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
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

//------ input.cpp -------//
void processInput( GLFWwindow *_window );
void mouse_callback(GLFWwindow* _window, double _xpos, double _ypos);
void scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset);
PixVec2 get_mouse_pos();

#endif
