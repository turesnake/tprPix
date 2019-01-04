/*
 * ========================= callback.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   有关 OpenGL 的一些 回调函数
 * ----------------------------
 */
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>


//------------------- 提供给全局的 函数 ----------------
void framebuffer_size_callback( GLFWwindow *_windowPtr, int _width, int _height );


/* ==========================================================
 *               framebuffer_size_callback
 * -----------------------------------------------------------
 * -- 当 目标window 的 framebuffer_size 改变时，此回调函数 会被调用。
 * -- 支持 运行时 动态调整 window 尺寸（拉伸 window）
 */
void framebuffer_size_callback( GLFWwindow *_windowPtr, int _width, int _height ){
	glViewport( 0, 0, _width, _height );
                //-- 此处的 参数 width, height 时 glfw 库 写入的，
}


