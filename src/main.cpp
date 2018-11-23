/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 * 
 *    main();
 *    
 * ----------------------------
 */
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- C ----------------------//
#include <string.h> //- memset, memcpy
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "wrapFuncs.h"
#include "MsgIO.h"

//------------------- SELF --------------------//
#include "globfunc.h"
#include "global.h"

#include "gl.h"
#include "rectangle.h" //-- 矩形 顶点数据

#include "ShaderProgram.h" 

 
using std::cout;
using std::endl;
using std::string;


//------------------- 从外部获得的 函数 ----------------



/* ===========================================================
 *                        main
 * -----------------------------------------------------------
 */
int main(){
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare();
    
    //==========================================//
    //                  TEST
    //------------------------------------------//

        
        // cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
        // return(0);
    //------------------------------------------//
    //                  glfw
    //------------------------------------------//
    glfw_init();
    glfw_hints_set();
    glfw_window_creat();
    glfw_oth_set();
    glfw_callback_set();

    //------------------------------------------//
    //                  glad
    //------------------------------------------//
    glad_init();
    glad_set();

    //---------------------------------------------//
    //                shader_program
    //---------------------------------------------//
    ShaderProgram rect_shader( 
                            "/shaders/base.vs",
                            "/shaders/base.fs"
                            );
    rect_shader.init();

    //--- 
    rect_shader.get_uniform_location( "model" );
    rect_shader.get_uniform_location( "view" );
    rect_shader.get_uniform_location( "projection" );

    //---------------------------------------------//
    //          创建／初始化  所有 模型 
    //---------------------------------------------//





    //---------------------------------------------//
    //           main render loop
    //---------------------------------------------//
    while( !glfwWindowShouldClose( window ) ){

        //------- 刷新 deltaTime ---------//
        update_time();

        //------- input 处理 -----------//
        //-- 目前这个版本 非常简陋
		processInput( window );

        //--------- 重绘背景 ---------
        glClearColor( 0.25f, 0.24f, 0.25f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                    //-- 在每一帧的新绘制之前，清除上一帧的 颜色缓冲 和 深度缓冲








        //-- check and call events and swap the buffers
		glfwPollEvents();   //-- 处理所有 处于 event queue 中的 待决 event。
		glfwSwapBuffers( window );

    }//---------- while render loop end -----------//

    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}







