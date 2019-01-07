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
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>


//----- callback.cpp -----
void framebuffer_size_callback( GLFWwindow *_windowPtr, int _width, int _height );


#endif
