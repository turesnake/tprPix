/*
 * ========================= gl_support.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   支援 main 函数的 子函数。
 * ----------------------------
 */
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

#include "pch.h"

//-------------------- Engine --------------------//
#include "input.h" 
#include "global.h"
#include "gl_funcs.h" 
#include "ViewingBox.h"
#include "esrc_window.h" 

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
    int ret = glfwInit();
    tprAssert( ret != GL_FALSE );
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

    tprAssert( ViewingBox::isFullScreen == false ); //-- 全屏模式 未完工
    if( ViewingBox::isFullScreen == true){
        //------ 全屏模式 ------//
        // 未完工... 禁止使用 
            tprAssert(0);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        esrc::set_windowPtr( glfwCreateWindow( ViewingBox::windowSZ.x,
                                            ViewingBox::windowSZ.y,
                                            "tprpix", 
                                            monitor, 
                                            nullptr ) );

    }else{
        //------ 窗口模式 ------//
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //- 阻止玩家在程序运行后，修改 window 尺寸

        esrc::set_windowPtr( glfwCreateWindow( ViewingBox::windowSZ.x,
                                            ViewingBox::windowSZ.y,
                                            "tprpix", 
                                            nullptr,  //-- moniter，若为 NULL ，表示 创建 “窗口模式”。
                                            nullptr ) );
    }

	if(esrc::get_windowPtr() == nullptr){
		glfwTerminate();
        tprAssert(0);
	}
    //-- 将这个 唯一的 window 设为 current context
	glfwMakeContextCurrent( esrc::get_windowPtr() );

}


/* ===========================================================
 *                  glfw_oth_set
 * -----------------------------------------------------------
 * -- glfw 一些杂乱选项的 设置
 */
void glfw_oth_set(){
    
    // 当本 window in forcus 后,鼠标不受任何影响
    // 就像操作一款常规软件，鼠标可显示在 窗口上方， 方便操作 窗口中的 ui图标
    glfwSetInputMode( esrc::get_windowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );

    //--- vsync --
    // 强制开启，在普通显示屏上，fps会被锁到 60

    // 但是好像会 失效 ...

    // 每次交换缓冲区之前等待的帧数：1
    glfwSwapInterval( 1 );
}


/* ===========================================================
 *                  glfw_callback_set
 * -----------------------------------------------------------
 * -- glfw 回调函数 设置区
 */
void glfw_callback_set(){
    // nothing ...
}


/* ===========================================================
 *                     glad_init  
 * -----------------------------------------------------------
 */
void glad_init(){

    if( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) ){
        tprAssert(0);
    }

    tprDebug::console( 
        "OpenGL: {0}. {1}",
        GLVersion.major, GLVersion.minor
    );


            // mac 4.1
            // win 0.0 -- 估计是有问题
}
 

/* ===========================================================
 *                     glad_set
 * -----------------------------------------------------------
 */
void glad_set(){
    
    //-------------------------//
    //          深度测试
    //-------------------------//
    glEnable(GL_DEPTH_TEST); 

    //-------------------------//
    //          混合
    //-------------------------//
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
    //   确保，先渲染所有的 实心图元，后渲染 半透明图元
    //
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    //-------------------------//
    //        模版检测
    //-------------------------//
    /*
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); 
	    // 只有当片段模版值为1，片段才会被渲染，否则被丢弃
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	    // 默认设置，不管做什么，模版缓冲都会保留 原先的值
	    // 新值不会被写入 模版缓冲
    glStencilMask(0x00); 
	    // 禁止向 模版缓冲 写入数据
        // -- 在这种设置下，模版检测几乎什么都不做
    */

}



