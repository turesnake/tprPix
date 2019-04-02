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

//-------------------- Engine --------------------//
#include "global.h"
#include "gl_funcs.h"
#include "input.h" 
#include "srcs_engine.h" //- 所有资源
#include "TimeCircle.h" 
#include "VAOVBO.h" 
#include "sectionBuild.h"
#include "fieldBorderSet_Handle.h"

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
    //               加载所有 资源
    //------------------------------------------//
        //return 0;

    //++++++ init ++++++//
    input::init_input();             //---- input -----
    init_VAOVBO();                   //---- VAO,VBO 资源 ----
    esrc::camera.init();             //---- camera 资源 ----
    esrc::gameSeed.init();           //---- gameSeed 资源 ---- tmp
    esrc::init_shaders();            //---- shaders 资源 ----
    //esrc::init_fieldBorderEntPixMaskSet();  //---- fieldBorderEntPixMaskSet 资源 ----
    esrc::init_colliEntSet_tables(); //---- ces_tables 资源 ----
    //init_globState_srcs();         //---- globState 资源 ----
        globState_byPass();

    //init_player_srcs();            //----  player 资源 ----
        //esrc::player.init();
        player_byPass();
    //...

    debug::init_debug();             //---- debug 资源 ----

    sectionBuild::init();            //----- sectionBuild 模块 --

    esrc::init_gpgpus();             //---- gpgpu 资源 ----

    esrc::init_canvases();           //---- canvas 资源 ----

    //++++++ load ++++++//
    esrc::load_colliEntSets(); //-- colliEntSets --

        //esrc::debug_colliEntSets();
    esrc::load_animFrameSets();      //-- animFrameSets --, MUST after load_colliEntSets()

    load_fieldBorderSets();          //----- fieldBorderSet ----

    esrc::init_ecoSyses();           //----- ecoSyses 资源 -----

    
    //...
        

    //---- 加载 map 数据 ----
    //...
        map_byPass(); //- 硬制作 一张 section map

        go_byPass();  //- 硬生产一组 Dog_A 实例

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

                //--- 定期检查 全局容器 esrc::chunkDeque
                // 如果存在需要 新建的 chunk，就新建一个（一次仅一个）
                // 如果5帧只创建一次，在go移动速度过快时，容易跟不上。所以多次调用
                sectionBuild::build_one_chunks_from_chunksDeque();
                
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

                //--- 定期检查 全局容器 esrc::chunkDeque
                // 如果存在需要 新建的 chunk，就新建一个（一次仅一个）
                // 如果5帧只创建一次，在go移动速度过快时，容易跟不上。所以多次调用
                sectionBuild::build_one_chunks_from_chunksDeque();

                break;
            case 3:

                //--- 定期 检查玩家所在 chunk
                //  并将需要新建的 chunks 收集到 队列中
                sectionBuild::collect_chunks_need_to_be_build_in_update_3();
                        // 更新中...

                break;
            case 4:
                //--- 定期检查 全局容器 esrc::chunkDeque
                // 如果存在需要 新建的 chunk，就新建一个（一次仅一个）
                // 如果5帧只创建一次，在go移动速度过快时，容易跟不上。所以多次调用
                sectionBuild::build_one_chunks_from_chunksDeque();

                break;
            default:
                assert(0);
        }


        //====================================//
        //          -- RENDER --
        //    Z-Deep 深的 mesh 必须先渲染
        //====================================//

        //--- clear RenderPools:
        // *** 注意次序 ***
        esrc::renderPool_meshs.clear();
        esrc::renderPool_goMeshs_pic.clear();
        esrc::renderPool_goMeshs_shadow.clear();
        

        //------------------------//
        //       chunks
        //------------------------//
        for( auto& p : esrc::chunks ){
            p.second.refresh_translate_auto(); //-- MUST !!!
            esrc::renderPool_meshs.insert({ p.second.mesh.get_render_z(),
                                                (Mesh*)&p.second.mesh });
        }

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
        esrc::draw_waterAnimCanvas();
        esrc::draw_renderPool_meshs(); 
        esrc::draw_renderPool_goMeshs_shadow();
        debug::draw_renderPool_mapEntSlices(); //-- 但是不在此文件中 clear
        debug::draw_renderPool_pointPics(); //-- 但是不在此文件中 clear
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


