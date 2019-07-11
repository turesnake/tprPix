/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

#include "cpp_with_csharp.h" //- tmp

//-------------------- Engine --------------------//
#include "prepare.h"
#include "global.h"
#include "gl_funcs.h"
#include "input.h" 
#include "TimeCircle.h" 
#include "VAOVBO.h" 
#include "chunkBuild.h"
#include "fieldBorderSet_Handle.h"
#include "dataBase.h"
#include "sceneLoop.h"
#include "esrc_all.h" //- 所有资源

 
//#include "tprDebug.h" //- tmp


/* ===========================================================
 *                     cppMain_tmp
 * -----------------------------------------------------------
 * 被 cs 调用
 * 在未来，要被分割
 */
int cppMain_tmp( char *exeDirPath_ ){
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare( exeDirPath_ );

    
    
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
        //cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
        //return(0);

    //------------------------------------------//
    //          init behaviour
    //          call_scriptMain
    //------------------------------------------//
    esrc::init_behaviour();
    esrc::call_scriptMain();

    //------------------------------------------//
    //        Behaviour.Awakes
    //------------------------------------------//
    esrc::get_behaviour().call_Awakes();

    //------------------------------------------//
    //           不依赖任何外部代码的资源
    //------------------------------------------//
    input::init_input();             //---- input -----
    esrc::init_time();               //---- timer,logicTimeCircle -----
    
    esrc::init_gameSeed();           //---- gameSeed ----
    esrc::init_gameArchive();        //---- gameArchive ----
    esrc::init_camera();             //---- camera 资源 ----
    esrc::init_shaders();            //---- shaders 资源 ----
    esrc::init_colliEntSet_tables(); //---- ces_tables 资源 ----
    esrc::init_chunks();             //---- chunks 模块的各种资源 ----

    db::atom_init_dataBase();        //---- dataBase 资源 ----
            //-- tmp...

    //------------------------------------------//
    //            启动 job线程组
    //------------------------------------------//
    esrc::start_jobThreads();


    //------------------------------------------//
    //                更多 资源
    //------------------------------------------//

                //-- 这里的一些资源，会被推迟到 不同的 scene 中...

    //++++++ init ++++++//
    init_VAOVBO();                   //---- VAO,VBO 资源 ----

    GameObj::id_manager.set_max_id( 0 );

    esrc::init_player();             //---- player -----
    //... 

    tprDebug::init_debug();             //---- tprDebug 资源 ----

    esrc::init_canvases();           //---- canvas 资源 ----

    //++++++ load ++++++//
    esrc::load_colliEntSets();       //-- colliEntSets --

    load_fieldBorderSets();          //----- fieldBorderSet ----
    esrc::init_ecoSysPlanes();       //----- ecoSysPlanes 资源 ----- MUST after esrc::behaviour.call_Awakes()
    //...
        
    
    //------------------------------------------//
    //        Behaviour.Starts
    //------------------------------------------//
    esrc::get_behaviour().call_Starts();

    //------------------------------------------//
    //           bind first scene
    //------------------------------------------//
    prepare_for_sceneBegin();

    //========================================================//
    //                 main render loop
    //========================================================//
    //GLFWwindow *targetWindowPtr = esrc::get_windowPtr();
    while( !glfwWindowShouldClose( esrc::get_windowPtr() ) ){

        //--------------------------------//
        //             time   
        //--------------------------------//
        esrc::get_timer().update_time();

        //--------------------------------//
        //            input   
        //--------------------------------//
        //-- 目前版本 非常简陋
		input::processInput( esrc::get_windowPtr() );

        //--------------------------------//
        //      render background   
        //--------------------------------//
        glClearColor( 0.25f, 0.24f, 0.25f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

        //================================//
        //       scene main loop      
        //       -- IMPORTANT --   
        //================================//
        sceneLoopFunc();

        //--------------------------------//
        //   check and call events
        //     swap the buffers               
        //--------------------------------//
		glfwPollEvents();          //-- 处理所有 处于 event queue 中的 待决event
		glfwSwapBuffers( esrc::get_windowPtr() ); //- 交换 两个 帧缓冲区

        //------------ 显示数据到终端 -----------//
        // [-DEBUG-]
        //...


    }//------------ while render loop end --------------------//
    db::atom_writeBack_to_table_gameArchive();
                    //-- 测试版位置，最终也许不放在这里


    //------------------------------------------//
    //            等待关闭 job线程组
    //   全局 唯一的 thread.join() 系列指令 添加处
    //------------------------------------------//
    esrc::join_jobThreads();


    //========================================================//
    //             save / delete everthing
    //--------------------------------------------------------//
    // 测试阶段，删不删无所谓
    db::atom_close_dataBase();          //------ 关闭 sqlite db -----
    delete_VAOVBO();           //------ 删除 全局唯一 VAO，VBO -----
    
    //...

    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}


