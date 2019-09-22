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

//-------------------- Engine --------------------//
#include "prepare.h"
#include "global.h"
#include "gl_funcs.h"
#include "input.h" 
#include "TimeCircle.h" 
#include "VAOVBO.h" 
#include "chunkCreate.h"
#include "fieldBorderSet_Handle.h"
#include "dataBase.h"
#include "sceneLoop.h"
#include "esrc_all.h" //- 所有资源

#include "timeLog.h" // debug_tmp

#include "tprDebug.h" //- tmp


/* ===========================================================
 *                        main
 * -----------------------------------------------------------
 */
int main( int argc, char* argv[] ){
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare( argv[0] );

    //------------------------------------------//
    //             ViewingBox
    //------------------------------------------//
    ViewingBox::init(); //--- MUST before OpenGL init !!!
    
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
    esrc::init_gameSeed();
    esrc::init_fields();
    esrc::init_gameArchive();
    esrc::init_camera();
    esrc::init_shaders();
    esrc::init_renderPools();
    esrc::init_chunks();             //---- chunks 模块的各种资源 ----
    esrc::init_chunkDatas();
    esrc::init_chunkMemStates();
    esrc::init_ecoObjs();
    esrc::init_gameObjs();

    db::atom_init_dataBase();
            //-- tmp...

    init_timeLog();  //- debug: timeLog 

    //------------------------------------------//
    //            启动 job线程组
    //------------------------------------------//
    esrc::start_jobThreads();



    //------------------------------------------//
    //                更多 资源
    //------------------------------------------//

                //-- 这里的一些资源，会被推迟到 不同的 scene 中...

    //++++++ init ++++++//
    init_VAOVBO();

    GameObj::id_manager.set_max_id( 0 );

    esrc::init_player();
    //... 

    tprDebug::init_debug();

    esrc::init_canvases();

    //++++++ load ++++++//
    load_fieldBorderSets();
    esrc::init_mapSurfaceRandSet();
    esrc::init_ecoSysPlanes();       // MUST after esrc::behaviour.call_Awakes()
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
    double logicLag = 0.0;
    auto &timerRef = esrc::get_timer();
    while( !glfwWindowShouldClose( esrc::get_windowPtr() ) ){

        //--------------------------------//
        //             time   
        //--------------------------------//
        timerRef.update_before_all(); //- MUST call first !!!

        //--------------------------------//
        //      render background   
        //--------------------------------//
        glClearColor( 0.25f, 0.24f, 0.25f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ); 
        //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

        //================================//
        // input/AI/LogicUpdate fps is steady,
        // Affected by TimeBase::logicUpdateTimeLimit
        logicLag += timerRef.get_last_deltaTime();
        while( logicLag >= TimeBase::logicUpdateTimeLimit ){
            logicLag -= TimeBase::logicUpdateTimeLimit;

            //--------------------------------//
            //            input   
            //--------------------------------//
            //-- 目前版本 非常简陋
            input::processInput( esrc::get_windowPtr() );

            //--------------------------------//
            //       scene Logic loop      
            //--------------------------------//
            sceneLogicLoopFunc();
        }

        //================================//
        //       scene Render loop       
        //================================//
        // render fps is not steady, 
        // Affected by vsync, monitor, GPU
        sceneRenderLoopFunc();

        //--------------------------------//
        //   check and call events
        //     swap the buffers               
        //--------------------------------//
		glfwPollEvents();          //-- handle all events in event queue
		glfwSwapBuffers( esrc::get_windowPtr() );


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
    //             timeLog debug
    //---------------------------------------------//
    process_and_echo_timeLog();

    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}


