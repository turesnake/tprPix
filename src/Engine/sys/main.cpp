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
//#include <iostream> //-- cout
#include <string>
#include <vector>

//-------------------- Engine --------------------//
#include "global.h"
#include "gl_funcs.h"
#include "srcs_engine.h" //- 所有资源
#include "TimeCircle.h" 

//------------------- Script --------------------//
#include "Script/byPass/byPass.h" //- tmp
 
//using std::cout;
//using std::endl;
using std::string;

//------------------- 从外部获得的 函数 [tmp] ----------------
extern void prepare();


inline int debugCount {0};
void debug_();


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

    //++++++ init ++++++//
    VAOVBO_init();         //---- VAO,VBO 资源 ----
    esrc::camera.init();         //---- camera 资源 ----
    esrc::shaders_init();        //---- shaders 资源 ----
    //globState_srcs_init(); //---- globState 资源 ----
        globState_byPass();

    //player_srcs_init();    //----  player 资源 ----
        player_byPass();
    //...



    //++++++ load ++++++//
    esrc::actions_load();    //-- actions --
    //...
        

    //---- 加载 map 数据 ----
    //...
        go_byPass();//- 硬生产一个 Dog_A 实例

    
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
        //-- 目前这个版本 非常简陋
		processInput( esrc::windowPtr );

        //--------------------------------//
        //      render background   
        //--------------------------------//
        glClearColor( 0.25f, 0.24f, 0.25f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                    //-- 在每一帧的新绘制之前，清除上一帧的 颜色缓冲 和 深度缓冲

        //--------------------------------//
        //    camera:: view, projection
        //--------------------------------//
        esrc::rect_shader.use_program();
        esrc::rect_shader.send_mat4_view_2_shader( esrc::camera.update_mat4_view() );
        esrc::rect_shader.send_mat4_projection_2_shader( esrc::camera.update_mat4_projection() );

        //--------------------------------//
        //           logic
        //--------------------------------//

            //auto itgo = esrc::memGameObjs.begin(); //- tmp
            //GameObj *gop;

        //-- 依据 逻辑时间循环，调用不同的 函数 --// 
        switch( esrc::logicTimeCircle.current() ){
            case 0:
                esrc::realloc_inactive_goes(); 

                /*
                itgo = esrc::memGameObjs.begin();
                for( ; itgo!=esrc::memGameObjs.end(); itgo++ ){
                    itgo->second.LogicUpdate( (GameObj*)&(itgo->second) );
                }
                */
                
                
                /*
                for( auto goid : esrc::goids_active ){
                    gop = (GameObj*)&(esrc::memGameObjs.at(goid));
                    gop->LogicUpdate();
                }
                */

                break;
            case 1:
                //debug_();
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
        
            auto it = esrc::memGameObjs.begin();
            for( ; it!=esrc::memGameObjs.end(); it++ ){
                assert( it->second.RenderUpdate != nullptr );
                it->second.RenderUpdate( (GameObj*)&(it->second) );
            }
        
        
        //--------------------------------//
        //   check and call events
        //     swap the buffers               
        //--------------------------------//
		glfwPollEvents();          //-- 处理所有 处于 event queue 中的 待决event
		glfwSwapBuffers( esrc::windowPtr ); //- 交换 两个 帧缓冲区

        //------------ 显示数据到终端 -----------//
        //cout << ".";
        // [-DEBUG-]
        //...


    }//------------ while render loop end --------------------//


    //========================================================//
    //             save / delete everthing
    //--------------------------------------------------------//
    VAOVBO_del();           //------ 删除 全局唯一 VAO，VBO -----
    //...


    //---------------------------------------------//
    //                glfw Terminate
    //---------------------------------------------//
    glfwTerminate();
    //------------//
    return(0);
}




void debug_(){
    

    //--------- only once -------------//
    debugCount++;
    if(debugCount != 5){
        return;
    }
    //--------- only once -------------//

    //cout << endl;

    /*
    auto it = esrc::memGameObjs.begin();
    for( ; it!=esrc::memGameObjs.end(); it++ ){

        it->second.debug();
    }
    */

}

