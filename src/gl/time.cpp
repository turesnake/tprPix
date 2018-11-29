/*
 * ========================= time.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   时间 帧 
 * ----------------------------
 */

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>


namespace{ //------------ namespace -----------//

    float deltaTime = 0.0f; //-- 当前帧 与 上一帧的 时间差
    float lastTime = 0.0f;  //-- 上一帧的 时间值
    float currentTime;     //-- 当前 时间值

} //----------------- namespace end -----------//


//------------------- 提供给全局的 函数 ----------------
void update_time();
float get_deltaTime();
float get_currentTime();


/* ==========================================================
 *                     update_time
 *-----------------------------------------------------------
 * -- 每帧 刷新时间
 */
void update_time(){
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime; //-- 更新 deltaTime
    lastTime = currentTime;
}


/* ==========================================================
 *                     get_deltaTime
 *-----------------------------------------------------------
 * -- 每帧 刷新时间
 */
float get_deltaTime(){
    return deltaTime;
}

/* ==========================================================
 *                     get_currentTime
 *-----------------------------------------------------------
 * -- 获得 当前时间 （从 glfw 启动 开始计算）
 */
float get_currentTime(){
    return glfwGetTime();
}

