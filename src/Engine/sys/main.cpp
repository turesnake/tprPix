/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *    main();
 * ----------------------------
 */
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- C ----------------------//
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>

//------------------- SELF --------------------//
#include "Engine/sys/global.h"
#include "Engine/gl/glob_gl.h"
#include "Engine/gl/RectVertics.h" //-- 矩形 顶点数据

#include "Engine/gl/ShaderProgram.h" 
#include "Engine/camera/Camera.h" 
#include "Engine/gl/Texture.h" 
#include "Engine/gl/Model.h"

#include "Engine/resource/srcs_manager.h" //- 所有资源
#include "Engine/time/TimeCircle.h" 

//------------------- Script --------------------//
#include "Script/byPass/byPass.h"
 
using std::cout;
using std::endl;
using std::string;

//------------------- 从外部获得的 函数 [tmp] ----------------
extern void prepare();


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

    //==========================================//
    //                  TEST
    //------------------------------------------//

        //Action as{ "/animal/dog_ack_01.P.png", 65, 112, 3, 4, 17, 2 };
        //as.init();
        
        //cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
        //return(0);

    //------------------------------------------//
    //          call_scriptMain
    //------------------------------------------//
    src::call_scriptMain();

    //------------------------------------------//
    //        Behaviour.Awakes
    //------------------------------------------//
    src::behaviour.call_Awakes();

    //------------------------------------------//
    //               加载所有 资源
    //------------------------------------------//

    //++++++ init ++++++//
    VAOVBO_init();         //---- VAO,VBO 资源 ----
    src::camera.init();         //---- camera 资源 ----
    src::shaders_init();        //---- shaders 资源 ----
    //globState_srcs_init(); //---- globState 资源 ----
        globState_byPass();

    //player_srcs_init();    //----  player 资源 ----
        player_byPass();
        
    //...



    //++++++ load ++++++//
    src::actions_load();    //-- actions --
    //...

    //---- 加载 map 数据 ----
    //...


    //---------------------------------------------//
    //          创建／初始化  所有 texture 
    //             [*** 将被废弃 ***]
    //---------------------------------------------//
    Texture textel_1( "/textures/pix_01.png" );

    //---------------------------------------------//
    //          创建／初始化  所有 model
    //             [*** 将被废弃 ***]
    //---------------------------------------------//

    Model mod_1;

    //-- 此矩形是 长宽为1 的基础矩形 --
    //-- 需要在每一帧，拉升为 目标尺寸 --
    /*
    mod_1.set_VBO( (GLvoid*)&(rect_base[0]),
                    (GLsizeiptr)(sizeof(float) * rect_base.size()),
                    (GLsizei)( sizeof(float) * 5 ) 
                    );
    */

    mod_1.add_texture( textel_1 );
    
    mod_1.set_shader_program( &src::rect_shader );

    mod_1.init();

    
    //---------------------------------------------//
    //                texture
    //             [*** 将被废弃 ***]
    //---------------------------------------------//
    //-- 必须要激活 shaderProgram，这样才能修改其 uniform 变量值。
    src::rect_shader.use_program();
    //-- 为 uniform 变量 texture1 设置值
    //-- 指向 GL_TEXTURE0 这两个 纹理单元
    glUniform1i( src::rect_shader.uniform_location( "texture1" ), 0);



    //------------------------------------------//
    //        Behaviour.Starts
    //------------------------------------------//
    src::behaviour.call_Starts();

    //========================================================//
    //                 main render loop
    //========================================================//
    while( !glfwWindowShouldClose( window ) ){

        //--------------------------------//
        //             time   
        //--------------------------------//
        src::timer.update_time();

        //--------------------------------//
        //            input   
        //--------------------------------//
        //-- 目前这个版本 非常简陋
		processInput( window );

        //--------------------------------//
        //      render background   
        //--------------------------------//
        glClearColor( 0.25f, 0.24f, 0.25f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                    //-- 在每一帧的新绘制之前，清除上一帧的 颜色缓冲 和 深度缓冲

        //--------------------------------//
        //    camera:: view, projection
        //--------------------------------//
        src::rect_shader.use_program();
        src::rect_shader.send_mat4_view_2_shader( src::camera.update_mat4_view() );
        src::rect_shader.send_mat4_projection_2_shader( src::camera.update_mat4_projection() );

        //--------------------------------//
        //           logic
        //--------------------------------//

        //-- 依据 逻辑时间循环，调用不同的 函数 --// 
        switch( src::logicTimeCircle.current() ){
            case 0:
                src::realloc_inactive_goes(); 
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                assert(0);
        }

        //--------------------------------//
        //        render graphic
        //--------------------------------//
        // 将被整合 ...
        
        //-- 每一次切换 动画动作，都将 矩形图元的长宽，拉升为对应的 像素值 --
        mod_1.set_translate( glm::vec3( 7.0f, 0.0f, 0.0f ) );
        mod_1.set_scale( glm::vec3( 16.0f, 16.0f, 1.0f ) );
        mod_1.model_draw();

        
        mod_1.set_translate( glm::vec3( -6.0f, 7.0f, 0.0f ) );
        mod_1.set_scale( glm::vec3( 16.0f, 16.0f, 1.0f ) );
        mod_1.model_draw();
        
        

        //--------------------------------//
        //   check and call events
        //     swap the buffers               
        //--------------------------------//
		glfwPollEvents();          //-- 处理所有 处于 event queue 中的 待决event
		glfwSwapBuffers( window ); //- 交换 两个 帧缓冲区

        //------------ 显示数据到终端 -----------//
        //...


    }//------------ while render loop end --------------------//
    //                delete everthing
    //========================================================//
    

    //mod_1.model_delete(); //------ 删除 所有 model ------
    VAOVBO_del();           //------ 删除 全局唯一 VAO，VBO -----
    //...


    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}







