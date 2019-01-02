/*
 * ========================= glob_gl.h ===================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 * 
 *   glfw / glad
 * 
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


//-- 一个 类实例 维护一个 window。
//-- window 数据结构的 具体内容由 glfw库管理。
//-- 用户只需保存一个指针。用来访问这个 window。
inline GLFWwindow  *window {};

//------ gl_support.cpp -----//
void glfw_init();
void glfw_hints_set();
void glfw_window_creat();
void glfw_oth_set();
void glfw_callback_set();

void glad_init();
void glad_set();


//------ input -------//
void processInput( GLFWwindow *window );
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
PixVec2 get_mouse_pos();

#endif
