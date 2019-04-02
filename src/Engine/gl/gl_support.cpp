/*
 * ========================= main_support.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   支援 main 函数的 子函数。
 * ----------------------------
 */
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- C ----------------------//
#include <cassert> //-- assert。

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "input.h" 
#include "srcs_engine.h" 
#include "global.h"
#include "config.h" //-- SCR_WIDTH
#include "callback.h" 
#include "gl_funcs.h" 

using std::string;

//#include "debug.h" //- tmp


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

    assert( IS_FULL_SCREEN == false ); //-- 全屏模式 未完工
    if( IS_FULL_SCREEN == true){
        //------ 全屏模式 ------//
        // 未完工...
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        esrc::windowPtr = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, 
                                    "tprcraft", 
                                    monitor, 
                                    NULL );
    }else{
        //------ 窗口模式 ------//
        // 暂不知 如何设置 帧率...
        esrc::windowPtr = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, 
                                    "tprcraft", 
                                    NULL,  //-- moniter，若为 NULL ，表示 创建 “窗口模式”。
                                    NULL );
    }

	if(esrc::windowPtr == NULL){
		glfwTerminate();
        assert(0);
	}
    //-- 将这个 唯一的 window 设为 current context
	glfwMakeContextCurrent( esrc::windowPtr );


    //------ 记录 “真实” 视口尺寸 ------//
    glfwGetFramebufferSize( esrc::windowPtr, 
                            &esrc::windowFrameBufferWH.x, 
                            &esrc::windowFrameBufferWH.y );

}


/* ===========================================================
 *                  glfw_oth_set
 * -----------------------------------------------------------
 * -- glfw 一些杂乱选项的 设置
 */
void glfw_oth_set(){
    //-- 当本 window in forcus 后，隐藏鼠标
    //  本游戏中，鼠标只影响 player_go 的方向，可以被 显示。
    //  所以，本游戏 不用 隐藏鼠标 
    //glfwSetInputMode(esrc::windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/* ===========================================================
 *                  glfw_callback_set
 * -----------------------------------------------------------
 * -- glfw 回调函数 设置区
 */
void glfw_callback_set(){

    //-- 目前版本并未控制 “当window拉升” 后的渲染效果。暂时，请勿拉升画面 ！！！ ---
    //...
    
    //glfwSetFramebufferSizeCallback( esrc::windowPtr, framebuffer_size_callback ); //-- 用户 更改 窗口尺寸。
    //glfwSetCursorPosCallback(       esrc::windowPtr, input::mouse_callback );  //-- 鼠标运动 -- 控制视角
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
    
    //-- 开启深度测试 --
    glEnable(GL_DEPTH_TEST); 

    //-- 支持 png texture 的半透明渲染 --
    //- 但这么设置完还是不完整。
    //   当一个 前方半透明物体先渲染，一个深处半透明物体再渲染时。后方物体将被遮挡。
    //   半透明失效。
    //   唯一的办法就是 先渲染后方(深处)的，再渲染前方的。
    //   -----------
    //   但是这么做非常麻烦。
    //   所以最好的办法就是：
    //     -- 让大部分 图元是 “要么实心，要么全透明”，然后通过 fs中，清除 alpha<0.01f 的像素 来实现
    //                绝大部分 图元 都应该属于这个类型
    //     -- 少部分 半透明 图元。单独处理。使用上述的 按次序渲染法，
    //
    //   确保，先渲染所有的 “非半透明”图元集，后渲染 半透明图元集
    //
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}



