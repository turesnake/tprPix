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
#include "chunkCreate.h"
#include "dataBase.h"
#include "sceneLoop.h"

#include "blueprint.h"

#include "GoSpecFromJson.h"
#include "UIGoSpecFromJson.h"

#include "esrc_all.h"
#include "ubo_all.h"

#include "timeLog.h" // debug_tmp
#include "speedLog.h" // debug_tmp
#include "tprDebug.h" //- tmp

//-------------------- Script --------------------//
#include "Script/json/json_all.h"


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

    esrc::init_coordinate();

    esrc::call_scriptMain();

    //------------------------------------------//
    //        init before Awake
    //------------------------------------------//
    json::parse_goJsonFile();
    json::parse_uiGoJsonFile();
    

    //------------------------------------------//
    //        Behaviour.Awakes
    //------------------------------------------//
    esrc::get_behaviour().call_Awakes();

    //------------------------------------------//
    //           不依赖任何外部代码的资源
    //------------------------------------------//
    
    input::init_input();
    esrc::init_time();               //---- timer,logicTimeCircle -----
    esrc::init_gameSeed();
    esrc::init_VAOVBO();
    
    esrc::init_colorTableSet();
    json::parse_colorTableJsonFile();
    

    esrc::init_fields();
    esrc::init_gameArchive();
    esrc::init_camera();
    esrc::init_uniformBlockObjs();
    esrc::init_renderPools();
    esrc::init_chunks();
    esrc::init_job_chunks();
    esrc::init_ecoObjMemStates();
    esrc::init_chunkMemStates();
    esrc::init_ecoObjs();
    esrc::init_gameObjs();

    db::atom_init_dataBase();
            //-- tmp...

    tprDebug::init_timeLog();
    tprDebug::init_speedLog();
    tprDebug::init_debug();

    //------------------------------------------//
    //            启动 job线程组
    //------------------------------------------//
    esrc::start_jobThreads();

    //------------------------------------------//
    //           more resources
    //------------------------------------------//
    GameObj::id_manager.set_max_id( 0 );

    esrc::init_shaders();
    esrc::init_player();
    
    esrc::init_canvases();

    json::parse_animFrameSetJsonFile(); // tmp

    blueprint::init_blueprint();

    esrc::init_ecoSysPlanes();
    
    
    //------------------------------------------//
    //        Behaviour.Starts
    //------------------------------------------//
    esrc::get_behaviour().call_Starts();

    //------------------------------------------//
    //            ubo [only once]
    //------------------------------------------//
    ubo::write_ubo_Camera();
    ubo::write_ubo_Window();
    ubo::write_ubo_WorldCoord();
    ubo::write_ubo_OriginColorTable();
    ubo::write_ubo_GroundColorTable();
    
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
        
        //--------------------------------//
        //            input   
        //--------------------------------//
        //-- 目前版本 非常简陋
        input::processInput( esrc::get_windowPtr() );

        //--------------------------------//
        //       scene Logic loop      
        //--------------------------------//
        sceneLogicLoopFunc();

        /*
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
            //input::processInput( esrc::get_windowPtr() );

            //--------------------------------//
            //       scene Logic loop      
            //--------------------------------//
            sceneLogicLoopFunc();
        }
        */
                        // 暂时停用，排查卡顿原因中 ...


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
    esrc::delete_VAOVBO();           //------ 删除 全局唯一 VAO，VBO -----
    
    //...

    //---------------------------------------------//
    //             timeLog debug
    //---------------------------------------------//

    tprDebug::process_and_echo_timeLog();
    tprDebug::process_and_echo_speedLog();

    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}


