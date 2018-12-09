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


//------------------- SELF --------------------//
#include "globfunc.h"
#include "global.h"

#include "glob_gl.h"
#include "RectVertics.h" //-- 矩形 顶点数据

#include "ShaderProgram.h" 
#include "Camera.h" 
#include "Texture.h" 
#include "Model.h"

#include "game_srcs.h"

 
using std::cout;
using std::endl;
using std::string;


//------------------- 从外部获得的 函数 ----------------
extern void test_1();


/* ===========================================================
 *                        main
 * -----------------------------------------------------------
 */
int main(){
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare();
    
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

    //------------------------------------------//
    //               加载所有 资源
    //------------------------------------------//
    action_srcs_add();

    //==========================================//
    //                  TEST
    //------------------------------------------//

        //Action_SRC as{ "/animal/dog_ack_01.P.png", 65, 112, 3, 4, 17, 2 };
        //as.init();
        
        //cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
        //test_1();
        //return(0);


    //---------------------------------------------//
    //          创建／初始化  所有 texture 
    //---------------------------------------------//
    Texture textel_1( "/textures/pix_01.png" );


    //---------------------------------------------//
    //                shader_program
    //---------------------------------------------//
    ShaderProgram rect_shader( "/shaders/base.vs",
                               "/shaders/base.fs" );
    rect_shader.init();

    //--- 
    rect_shader.get_uniform_location( "model" );
    rect_shader.get_uniform_location( "view" );
    rect_shader.get_uniform_location( "projection" );

    rect_shader.get_uniform_location( "texture1" );


    rect_shader.use_program();


    
    //---------------------------------------------//
    //          创建／初始化  所有 model
    //---------------------------------------------//

    Model mod_1;

    //-- 此矩形是 长宽为1 的基础矩形 --
    //-- 需要在每一帧，拉升为 目标尺寸 --
    mod_1.set_VBO( (GLvoid*)&(rect_base[0]),
                    (GLsizeiptr)(sizeof(float) * rect_base.size()),
                    (GLsizei)( sizeof(float) * 5 ) 
                    );

    
    mod_1.add_texture( textel_1 );
    
    mod_1.set_shader_program( &rect_shader );

    mod_1.init();

    
    //---------------------------------------------//
    //                texture
    //---------------------------------------------//
    //-- 必须要激活 shaderProgram，这样才能修改其 uniform 变量值。
    rect_shader.use_program();
    //-- 为 两个 uniform 变量 texture1,texture2  设置值。
    //-- 分别指向 GL_TEXTURE0，GL_TEXTURE1 这两个 纹理单元。
    glUniform1i( rect_shader.uniform_location( "texture1" ), 0);
    //glUniform1i( rect_shader.uniform_location( "texture2" ), 1);


    //---------------------------------------------//
    //                 camera
    //---------------------------------------------//
    Camera camera; //-- 本游戏暂时只有 一个 摄像机
    camera.init();
    bind_camera_current( &camera ); //-- 将其绑定为 当前摄像机


    //---------------------------------------------//
    //           main render loop
    //---------------------------------------------//
        //u64 print_counter = 0;
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


        //------------------------------------------//
        //                 
        //------------------------------------------//
        rect_shader.use_program();
        rect_shader.send_mat4_view_2_shader( 
                                camera_current()->update_mat4_view()
                                );

        rect_shader.send_mat4_projection_2_shader(
                                camera_current()->update_mat4_projection()
                                );

        //-------------------- 绘制图形 -----------------------

        //-- 每一次切换 动画动作，都将 矩形图元的长宽，拉升为对应的 像素值 --
        mod_1.set_scale( glm::vec3( 16.0f, 16.0f, 1.0f ) );
        mod_1.model_draw();


        //-- check and call events and swap the buffers
		glfwPollEvents();   //-- 处理所有 处于 event queue 中的 待决 event。
		glfwSwapBuffers( window );

        //------------ 显示数据到终端 -----------//

            /*
            print_counter++;
            glm::vec3 pos;
            if( (print_counter%100) == 0 ){
                pos = camera_current()->get_cameraPos();
                cout << pos.x << ", "
                    << pos.y << ", "
                    << pos.z
                    << endl;
            }
            */

    }//---------- while render loop end -----------//


    //------------ 删除 所有 model -------------
    mod_1.model_delete();

    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}







