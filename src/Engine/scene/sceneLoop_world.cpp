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
#include "tprAssert.h"
#include "input.h"
#include "chunkCreate.h"
#include "chunkRelease.h"
#include "esrc_all.h" //- 所有资源

#include "tprDebug.h"


using namespace std::placeholders;


namespace sc_world_inn {//-------------- namespace: sc_world_inn ------------------//


    //-- 临时数据 --
    bool  isOld_A_press {false}; //- 上一帧，A键是否按下
    bool  isOld_B_press {false}; //- 上一帧，B键是否按下
    bool  isOld_X_press {false}; //- 上一帧，X键是否按下
    bool  isOld_Y_press {false}; //- 上一帧，Y键是否按下


    bool  isNew_A_press {false};
    bool  isNew_B_press {false};
    bool  isNew_X_press {false};
    bool  isNew_Y_press {false};


    //===== funcs =====//
    void inputINS_handle_in_sceneWorld( const InputINS &inputINS_);

}//------------------ namespace: sc_world_inn end ------------------//




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
        //chunkCreate::build_9_chunks( IntVec2{ 1,1 } );

        //go_byPass();  //- 硬生产一组 Norman 实例

        //esrc::player.bind_goPtr(); //-- 务必在 go数据实例化后 再调用 --

    
    esrc::get_camera().set_allDPos( esrc::get_player().get_goRef().goPos.get_currentDPos() );
    input::bind_inputINS_handleFunc( std::bind( &sc_world_inn::inputINS_handle_in_sceneWorld, _1 ) );

    switch_sceneLoop( SceneLoopType::World );
}


/* ===========================================================
 *                    sceneLogicLoop_world
 * -----------------------------------------------------------
 */
void sceneLogicLoop_world(){

    //--------------------------------//
    //           logic
    //--------------------------------//
    //-- 依据 逻辑时间循环，调用不同的 函数 --// 
    switch( esrc::get_logicTimeCircle().current() ){
        case 0:
            esrc::realloc_inactive_goes(); //- tmp
            break;
        case 1:
            esrc::foreach_goids_active(
                []( goid_t goid_, GameObj &goRef_ ){

                    goRef_.LogicUpdate( goRef_ );
                            //-- 这么设计还是会造成 拥塞问题
                            //   大量的 go在同一帧 更新自己的 logic。
                            //   最好的办法是，分摊到 不同的帧中去...
                }
            );
            break;
        case 2:
            //--- 定期 检查玩家所在 chunk
            //  并将需要新建的 chunks 收集到 队列中
            chunkCreate::collect_chunks_need_to_be_build_in_update();
                        // 更新中...
            break;
        case 3:
            //--- 定期 检查玩家所在 chunk
            // 当发现 chunkReleaseZone 发生位移时，将需要 释放的chunk 排入队列
            chunkRelease::collect_chunks_need_to_be_release_in_update();

            
            break;
        case 4:
            //esrc::realloc_active_goes(); //- tmp
            break;
        default:
            tprAssert(0);
    }

    //--------------------------------//
    //  每一帧，最多装配生成一个 chunk 实例（如果有）
    //  或者 释放一个 chunk 实例（如果有）
    //--------------------------------//
    auto pairRet = chunkCreate::chunkBuild_3_receive_data_and_build_one_chunk();
    if( pairRet.first == false ){
        chunkRelease::release_one_chunk();
    }
                    // 一种临时的，简陋的 平衡帧间开销的方式
                    // 在未来，需要实现一个专门的平衡器，来分配 本帧 的工作
                    // 确保主线程的工作 不拥堵
                    // ...

}


/* ===========================================================
 *                    sceneRenderLoop_world
 * -----------------------------------------------------------
 */
void sceneRenderLoop_world(){

    //--------------------------------//
    //    camera:: RenderUpdate()
    //    camera --> shader: view, projection
    //--------------------------------//
    esrc::get_camera().RenderUpdate();
    //--- 
    ShaderProgram &rect_shaderRef = esrc::get_rect_shader();
    rect_shaderRef.use_program();
    rect_shaderRef.send_mat4_view_2_shader( esrc::get_camera().update_mat4_view() );
    rect_shaderRef.send_mat4_projection_2_shader( esrc::get_camera().update_mat4_projection() );

    //====================================//
    //          -- RENDER --
    //    Z-Deep 深的 mesh 必须先渲染
    //====================================//

    //--- clear RenderPools:
    // *** 注意次序 ***
    esrc::clear_renderPool_meshs();
    esrc::clear_renderPool_goMeshs_pic();
    esrc::clear_renderPool_goMeshs_shadow();
    esrc::clear_renderPool_mapSurfaces();

    //------------------------//
    //        chunks
    //------------------------//
    esrc::add_chunks_2_renderPool();

    //------------------------//
    //     mapEntSlices
    //------------------------//
    //...

    //------------------------//
    //     - shadowMeshs
    //     - picMeshs
    //------------------------//
    esrc::foreach_goids_active(
        []( goid_t goid_, GameObj &goRef_ ){
            
            tprAssert( goRef_.RenderUpdate != nullptr );
            goRef_.RenderUpdate( goRef_ ); 
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
    tprDebug::draw_renderPool_mapEntSlices();  //-- tprDebug 但是不在此文件中 clear
    tprDebug::draw_renderPool_pointPics();     //-- tprDebug 但是不在此文件中 clear
    esrc::draw_renderPool_goMeshs_pic(); 

}




namespace sc_world_inn {//-------------- namespace: sc_world_inn ------------------//


/* ===========================================================
 *              inputINS_handle_in_sceneWorld
 * -----------------------------------------------------------
 */
void inputINS_handle_in_sceneWorld( const InputINS &inputINS_){

    Player &playerRef = esrc::get_player();
    GameObj &playerGoRef = playerRef.get_goRef();
    //-----------------//
    //      camera
    //-----------------//
    //-- 让 camera 对其上1渲染帧 --
    //- 这会造成 camera 的延迟，但不要紧
    esrc::get_camera().set_targetDPos( playerGoRef.goPos.get_currentDPos() );

    //... 暂时没有 处理 剩余功能键的 代码 

    //-- 直接传递给 player
    playerRef.handle_inputINS( inputINS_ );

    //-----------------//
    //      tmp
    // 用 A 键 来增加 playerGo speed
    // 用 B 键 减速
    //-----------------//
    isNew_A_press = false;
    isNew_B_press = false;
    isNew_X_press = false;
    isNew_Y_press = false;

    if( inputINS_.check_key(GameKey::KEY_A) ){
        isNew_A_press = true;
    }
    if( inputINS_.check_key(GameKey::KEY_B) ){
        isNew_B_press = true;
    }
    if( inputINS_.check_key(GameKey::KEY_X) ){
        isNew_X_press = true;
    }
    if( inputINS_.check_key(GameKey::KEY_Y) ){
        isNew_Y_press = true;
    }

    SpeedLevel lvl = playerGoRef.move.get_speedLvl();
    //-- 有效的 节点帧 --
    if( (isOld_A_press==false) && (isNew_A_press) ){
        SpeedLevel newLvl = calc_higher_speedLvl(lvl);
        playerGoRef.move.set_speedLvl( newLvl );
            cout << " + " << static_cast<int>(newLvl) 
                << ", " << SpeedLevel_2_val(newLvl)
                << endl; 
    }
    if( (isOld_B_press==false) && (isNew_B_press) ){
        SpeedLevel newLvl = calc_lower_speedLvl(lvl);
        playerGoRef.move.set_speedLvl( newLvl );
            cout << " - " << static_cast<int>(newLvl) 
                << ", " << SpeedLevel_2_val(newLvl)
                << endl;
    }
    if( (isOld_X_press==false) && (isNew_X_press) ){

        const auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( playerGoRef.goPos.get_currentMPos() );
        const auto &field = esrc::atom_get_field( anyMPos_2_fieldKey(mapEntRef.get_mpos()) );

        //IntVec2 nodeMPosOff = field.get_nodeMPos() - field.get_mpos(); //- 未被使用...
        cout << "mapAlti.val = " << mapEntRef.mapAlti.val
            //<< ";   fieldKey = " << mapEntPtr->fieldKey
            << ";   nodeFieldAltiVal = " << field.get_nodeMapAlti().val
            << ";   minAltiVal = " << field.get_minMapAlti().val
            << ";   maxAltiVal = " << field.get_maxMapAlti().val
            //<< ";   nodeMPosOff = " << nodeMPosOff.x
            //<< "," << nodeMPosOff.y
            << endl;

    }
    if( (isOld_Y_press==false) && (isNew_Y_press) ){
        //-- 暂时什么都不做 ...

        esrc::chunks_debug();

    }

    //---------------
    isOld_A_press = isNew_A_press;
    isOld_B_press = isNew_B_press;
    isOld_X_press = isNew_X_press;
    isOld_Y_press = isNew_Y_press;
}


}//------------------ namespace: sc_world_inn end ------------------//














