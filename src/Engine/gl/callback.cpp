/*
 * ========================= callback.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   有关 OpenGL 的一些 回调函数
 * ----------------------------
 */
#include "callback.h" 

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


