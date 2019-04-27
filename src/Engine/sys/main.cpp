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

//-------------------- C ----------------------//
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <string>
#include <vector>
//#include <thread>

//-------------------- Engine --------------------//
#include "global.h"
#include "gl_funcs.h"
#include "input.h" 
#include "TimeCircle.h" 
#include "VAOVBO.h" 
#include "sectionBuild.h"
#include "fieldBorderSet_Handle.h"
#include "esrc_all.h" //- 所有资源

//------------------- Script --------------------//
#include "Script/byPass/byPass.h" //- tmp
 
using std::string;

#include "debug.h" //- tmp

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
        //cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
        //return(0);

    //------------------------------------------//
    //          call_scriptMain
    //------------------------------------------//
    esrc::call_scriptMain();

    //------------------------------------------//
    //        Behaviour.Awakes
    //------------------------------------------//
    esrc::behaviour.call_Awakes();

    //------------------------------------------//
    //           不依赖任何外部代码的资源
    //------------------------------------------//
    input::init_input();             //---- input -----
    esrc::camera.init();             //---- camera 资源 ----
    esrc::init_shaders();            //---- shaders 资源 ----
    esrc::init_colliEntSet_tables(); //---- ces_tables 资源 ----




    //------------------------------------------//
    //            启动 job线程组
    //------------------------------------------//
    esrc::start_jobThreads();


    //------------------------------------------//
    //                更多 资源
    //------------------------------------------//
    //++++++ init ++++++//
    init_VAOVBO();                   //---- VAO,VBO 资源 ----
    esrc::gameSeed.init();           //---- gameSeed 资源 ---- tmp
    //init_globState_srcs();         //---- globState 资源 ----
        globState_byPass();

    //init_player_srcs();            //----  player 资源 ----
        //esrc::player.init();
        player_byPass();
    //...

    debug::init_debug();             //---- debug 资源 ----

    //esrc::init_gpgpus();             //---- gpgpu 资源 ----
    esrc::init_canvases();           //---- canvas 资源 ----

    //++++++ load ++++++//
    esrc::load_colliEntSets(); //-- colliEntSets --

    esrc::load_animFrameSets();      //-- animFrameSets --, MUST after load_colliEntSets()

    load_fieldBorderSets();          //----- fieldBorderSet ----

    esrc::init_ecoSyses();           //----- ecoSyses 资源 ----- MUST after esrc::behaviour.call_Awakes()

    //...
        

    //---- 加载 map 数据 ----
    //...

        //--- 最简模式，仅仅生成 玩家所在的 chunk 及其周边 9 个 chunk
        //   在未来，会被 完善的 游戏存档系统 所取代
        sectionBuild::build_9_chunks( IntVec2{ 1,1 } );


        go_byPass();  //- 硬生产一组 Norman 实例


        esrc::player.bind_goPtr(); //-- 务必在 go数据实例化后 再调用 --

    
    //------------------------------------------//
    //        Behaviour.Starts
    //------------------------------------------//
    esrc::behaviour.call_Starts();

    //========================================================//
    //                 main render loop
    //========================================================//
    while( !glfwWindowShouldClose( esrc::windowPtr ) ){

        //--------------------------------//
        //             time   
        //--------------------------------//
        esrc::timer.update_time();

        //--------------------------------//
        //            input   
        //--------------------------------//
        //-- 目前版本 非常简陋
		input::processInput( esrc::windowPtr );

        //--------------------------------//
        //      render background   
        //--------------------------------//
        glClearColor( 0.25f, 0.24f, 0.25f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                    //-- 在每一帧的新绘制之前，清除上一帧的 颜色缓冲 和 深度缓冲

        //--------------------------------//
        //    camera:: RenderUpdate()
        //    camera --> shader: view, projection
        //--------------------------------//
        esrc::camera.RenderUpdate();
        //--- 
        esrc::rect_shader.use_program();
        esrc::rect_shader.send_mat4_view_2_shader( esrc::camera.update_mat4_view() );
        esrc::rect_shader.send_mat4_projection_2_shader( esrc::camera.update_mat4_projection() );

        //--------------------------------//
        //           logic
        //--------------------------------//

        //-- 依据 逻辑时间循环，调用不同的 函数 --// 
        switch( esrc::logicTimeCircle.current() ){
            case 0:
                esrc::realloc_inactive_goes(); //- tmp
                
                break;

            case 1:
                esrc::foreach_goids_active(
                    []( goid_t _goid, GameObj *_goPtr ){
                        _goPtr->LogicUpdate( _goPtr );
                    }
                );
                break;

            case 2:
                //esrc::camera.print_pos();


                break;
            case 3:

                //--- 定期 检查玩家所在 chunk
                //  并将需要新建的 chunks 收集到 队列中
                sectionBuild::collect_chunks_need_to_be_build_in_update();
                        // 更新中...

                break;
            case 4:

                break;
            default:
                assert(0);
        }

        //--------------------------------//
        //  每一帧，最多装配生成一个 chunk 实例
        //--------------------------------//
        sectionBuild::chunkBuild_3_receive_data_and_build_one_chunk();


        //====================================//
        //          -- RENDER --
        //    Z-Deep 深的 mesh 必须先渲染
        //====================================//

        //--- clear RenderPools:
        // *** 注意次序 ***
        esrc::renderPool_meshs.clear();
        esrc::renderPool_goMeshs_pic.clear();
        esrc::renderPool_goMeshs_shadow.clear();
        esrc::renderPool_mapSurfaces.clear();
        

        //------------------------//
        //       chunks
        //------------------------//
        esrc::render_chunks();

        //------------------------//
        //     mapEntSlices
        //------------------------//
        //...


        //------------------------//
        //     - shadowMeshs
        //     - picMeshs
        //------------------------//
        esrc::foreach_goids_active(
            []( goid_t _goid, GameObj *_goPtr ){
                assert( _goPtr->RenderUpdate != nullptr );
                _goPtr->RenderUpdate( _goPtr ); 
            }
        );


        //>>>>>>>>>>>>>>>>>>>>>>>>//
        //        draw call
        //>>>>>>>>>>>>>>>>>>>>>>>>//
        // *** 注意次序,先渲染深处的 ***
        esrc::draw_groundCanvas();
        esrc::draw_renderPool_meshs(); //- chunks
        esrc::draw_waterAnimCanvas();
        esrc::draw_renderPool_mapSurfaces();
        esrc::draw_renderPool_goMeshs_shadow();
        debug::draw_renderPool_mapEntSlices();  //-- debug 但是不在此文件中 clear
        debug::draw_renderPool_pointPics();     //-- debug 但是不在此文件中 clear
        esrc::draw_renderPool_goMeshs_pic(); 

        //--------------------------------//
        //   check and call events
        //     swap the buffers               
        //--------------------------------//
		glfwPollEvents();          //-- 处理所有 处于 event queue 中的 待决event
		glfwSwapBuffers( esrc::windowPtr ); //- 交换 两个 帧缓冲区

        //------------ 显示数据到终端 -----------//
        // [-DEBUG-]
        //...


    }//------------ while render loop end --------------------//


    //------------------------------------------//
    //            等待关闭 job线程组
    //   全局 唯一的 thread.join() 系列指令 添加处
    //------------------------------------------//
    esrc::join_jobThreads();


    //========================================================//
    //             save / delete everthing
    //--------------------------------------------------------//
    // 测试阶段，删不删无所谓
    VAOVBO_del();           //------ 删除 全局唯一 VAO，VBO -----
    //...


    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}


