/*
 * ========================= callback.h ==========================
 *                            -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *  有关 OpenGL 的一些 回调函数
 * ----------------------------
 */
#ifndef _TPR_CALL_BACK_H_
#define _TPR_CALL_BACK_H_


/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>


//----- callback.cpp -----
void framebuffer_size_callback( GLFWwindow *window, int width, int height );



#endif
