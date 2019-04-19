/*
 * ========================= esrc_window.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_WINDOW_H_
#define _TPR_ESRC_WINDOW_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- Engine --------------------//
#include "IntVec.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//


//-- 一个 类实例 维护一个 window。
//-- window 数据结构的 具体内容由 glfw库管理。
//-- 用户只需保存一个指针。用来访问这个 window。
inline GLFWwindow  *windowPtr {};

//-- 游戏窗口 视口实际尺寸（pix）
// 在 mac 这种高分屏中，系统实际为程序分配的 窗口尺寸，可能不是 SCR_WIDTH,SCR_HEIGHT 这组值
// 而是它的 2 倍。
// 这组值在 glViewport() 中会被用到。
// 推荐的做法：先通过 glfwGetFramebufferSize(), 检查出当前的 真实视口尺寸
// 再用这个 尺寸去 改写 glViewport()
inline IntVec2 windowFrameBufferWH {};


}//---------------------- namespace: esrc -------------------------//
#endif

