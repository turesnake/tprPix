/*
 * ========================= main_support.cpp ==========================
 *                          -- GL_CPP --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   支援 main 函数的 子函数。
 * ----------------------------
 */

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- C ----------------------//
#include <cassert> //-- assert。

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>


//------------------- SELF --------------------//
#include "globfunc.h"
#include "global.h"
#include "config.h" //-- SCR_WIDTH
#include "callback.h" 
#include "gl.h" 

using std::cout;
using std::endl;
using std::string;


//------------------- 提供给全局的 函数 ----------------
void glfw_init();
void glfw_hints_set();
void glfw_window_creat();
void glfw_oth_set();
void glfw_callback_set();

void glad_init();
void glad_set();



/* ===========================================================
 *                     glfw_init
 * -----------------------------------------------------------
 * -- glfw 库 的初始化。
 */
void glfw_init(){
    assert( glfwInit() != GL_FALSE );
}


/* ===========================================================
 *                     glfw_hints_set
 * -----------------------------------------------------------
 */
void glfw_hints_set(){

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    #ifdef __APPLE__
	    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); //-- OSX 必须
                                //-- FORWARD_COMPAT: 向前兼容／向旧版本兼容
    #endif
}


/* ===========================================================
 *                  glfw_window_creat   
 * -----------------------------------------------------------
 */
void glfw_window_creat(){

	window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "tprcraft", NULL, NULL );

	if(window == NULL){
		glfwTerminate();
        cout << "glfw_window_creat: glfwCreateWindow: error. " << endl;
        assert(0);
	}
    //-- 将这个 唯一的 window 设为 current context
	glfwMakeContextCurrent( window );

}


/* ===========================================================
 *                  glfw_oth_set
 * -----------------------------------------------------------
 * -- glfw 一些杂乱选项的 设置
 */
void glfw_oth_set(){
     //-- 当本 window in forcus 后，隐藏鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/* ===========================================================
 *                  glfw_callback_set
 * -----------------------------------------------------------
 * -- glfw 回调函数 设置区
 */
void glfw_callback_set(){

    
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback ); //-- 用户 更改 窗口尺寸。
    //glfwSetCursorPosCallback(       window, mouse_callback );  //-- 鼠标运动 -- 控制视角
    //glfwSetScrollCallback(          window, scroll_callback ); //-- 鼠标滚轮 -- 控制视野


}


/* ===========================================================
 *                     glad_init  
 * -----------------------------------------------------------
 */
void glad_init(){
    assert( gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) );
}
 

/* ===========================================================
 *                     glad_set
 * -----------------------------------------------------------
 */
void glad_set(){
    glEnable(GL_DEPTH_TEST); //-- 开启深度测试
}














