/*
 * =================== sceneLoop_world.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   正式游戏的 sceneLoop
 * ----------------------------
 */
#include "sceneLoop.h"


//-------------------- Engine --------------------//
#include "input.h"
#include "chunkBuild.h"
#include "esrc_all.h" //- 所有资源


//------------------- Script --------------------//
#include "Script/byPass/byPass.h" //- tmp

using namespace std::placeholders;


namespace{//-------------- namespace ------------------//



    //===== funcs =====//
    void inputINS_handle_in_sceneWorld( const InputINS &_inputINS);

}//------------------ namespace: end ------------------//




/* ===========================================================
 *                prepare_for_sceneWorld
 * -----------------------------------------------------------
 * -- 在 siwth 进入 scene:world 之前，需要准备的工作
 */
void prepare_for_sceneWorld(){


    //-- 合理的流程应该是：
    //  先 读取 player 数据，还有更多 全局性数据
    //  然后根据这些数据，来 build chunks
    //
    //
    
        //--- 最简模式，仅仅生成 玩家所在的 chunk 及其周边 9 个 chunk
        //   在未来，会被 完善的 游戏存档系统 所取代
        //chunkBuild::build_9_chunks( IntVec2{ 1,1 } );

        //go_byPass();  //- 硬生产一组 Norman 实例

        //esrc::player.bind_goPtr(); //-- 务必在 go数据实例化后 再调用 --

    
    esrc::camera.set_allFPos( esrc::player.goPtr->goPos.get_currentFPos() );
    input::bind_inputINS_handleFunc( std::bind( &inputINS_handle_in_sceneWorld, _1 ) );

    switch_sceneLoop( SceneLoopType::World );
}


/* ===========================================================
 *                    sceneLoop_world
 * -----------------------------------------------------------
 */
void sceneLoop_world(){

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
            chunkBuild::collect_chunks_need_to_be_build_in_update();
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
    chunkBuild::chunkBuild_3_receive_data_and_build_one_chunk();


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
                     //-- 在未来，这个接口应该被改良...
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

}




namespace{//-------------- namespace ------------------//


/* ===========================================================
 *              inputINS_handle_in_sceneWorld
 * -----------------------------------------------------------
 */
void inputINS_handle_in_sceneWorld( const InputINS &_inputINS){

    //-----------------//
    //      camera
    //-----------------//
    //-- 让 camera 对其上1渲染帧 --
    //- 这会造成 camera 的延迟，但不要紧
    esrc::camera.set_targetFPos( esrc::player.goPtr->goPos.get_currentFPos() );


    //... 暂时没有 处理 剩余功能键的 代码 

    //-- 直接传递给 player
    esrc::player.handle_inputINS( _inputINS );

}


}//------------------ namespace: end ------------------//














