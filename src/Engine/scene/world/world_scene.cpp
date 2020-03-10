/*
 * ====================== world_scene.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   scene: world
 * 
 *   正式的 游戏世界
 * 
 * ----------------------------
 */
#include "pch.h"
#include "sceneLoop.h"

//-------------------- Engine --------------------//
#include "input.h"
#include "chunkCreate.h"
#include "chunkRelease.h"

#include "ubo_all.h"

#include "esrc_all.h" //- 所有资源


//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupColorTable.h"


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
    void inputINS_handle( const InputINS &inputINS_);

}//------------------ namespace: sc_world_inn end ------------------//




/* 在 进入 scene:world 之前，需要准备的工作
 */
void prepareForScene_world(){

    //-- 合理的流程应该是：
    //  先 读取 player 数据，还有更多 全局性数据
    //  然后根据这些数据，来 build chunks
    //
    //
        
    
    esrc::get_camera().set_allDPos( esrc::get_player().get_goRef().get_dpos() );
    input::bind_inputINS_handleFunc( std::bind( &sc_world_inn::inputINS_handle, _1 ) );

    switch_sceneLoop( SceneLoopType::World );


    //--------------------------------//
    //          ubo [one time]
    //--------------------------------//
    ubo::write_ubo_Seeds(); // Must in SceneWorld
    ubo::write_ubo_Camera();

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

                    //-- Only Major --
                    if( goRef_.family == GameObjFamily::Major ){ // BioSoup 是否要支持 ？？？
                        goRef_.LogicUpdate( goRef_ );
                            //-- 这么设计还是会造成 拥塞问题
                            //   大量的 go在同一帧 更新自己的 logic。
                            //   最好的办法是，分摊到 不同的帧中去...
                    }
                }
            );
            break;
        case 2:
            //--- 定期 检查玩家所在 section
            //  并将需要新建的 ecoObjs 收集到 队列中
            chunkCreate::collect_ecoObjs_need_to_be_create();

            //--- 定期 检查玩家所在 chunk
            //  并将需要新建的 chunks 收集到 队列中
            chunkCreate::collect_chunks_need_to_be_create_in_update();
                        // 更新中...
            break;
        case 3:
            //--- 定期 检查玩家所在 chunk
            // 当发现 chunkReleaseZone 发生位移时，将需要 释放的chunk 排入队列
            chunkRelease::collect_chunks_need_to_be_release_in_update();

            //-- 释放距离过远的 ecoobj 
            esrc::del_ecoObjs_tooFarAway();

            break;
        case 4:
            esrc::realloc_active_goes(); //- tmp
            break;
        default:
            tprAssert(0);
    }

    //--------------------------------//
    // 及时将 已在 job线程中 创建完成的 ecoObj
    // move 到 总容器
    //--------------------------------//
    esrc::atom_move_all_ecoObjUptrs_from_job_2_esrc();

    //--------------------------------//
    // 及时将符合要求的 chunk 送入创建队列
    //--------------------------------//
    chunkCreate::create_chunks_from_waitingQue();

    //--------------------------------//
    //  每一帧，最多装配生成一个 chunk 实例（如果有）
    //  或者 释放一个 chunk 实例（如果有）
    //--------------------------------//
    auto chunkKeyOpt = chunkCreate::chunkCreate_3_receive_data_and_create_one_chunk();
    if( !chunkKeyOpt.has_value() ){
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
    //--------------------------------//
    esrc::get_camera().RenderUpdate();

    //--------------------------------//
    //             ubo
    //--------------------------------//
    ubo::write_ubo_Camera();
    ubo::write_ubo_Time();
    ubo::update_and_write_ubo_UnifiedColorTable();
    gameObjs::bioSoup::write_ubo_BioSoupColorTable();


    //====================================//
    //          -- RENDER --
    //    Z-Deep 深的 mesh 必须先渲染
    //====================================//
    //--- clear RenderPools:
    esrc::clear_all_renderPool();

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
            
            //--- RenderUpdate ---//
            tprAssert( goRef_.RenderUpdate != nullptr );
            goRef_.RenderUpdate( goRef_ ); 

            //--- historyVals ---//
            goRef_.make_sure_all_historyVals_is_synced();
        }
    );

    //>>>>>>>>>>>>>>>>>>>>>>>>//
    //        draw call
    //>>>>>>>>>>>>>>>>>>>>>>>>//
    esrc::draw_groundCanvas();
    esrc::get_groundRenderPool().draw();

        //tprDebug::draw_renderPool_mapEntSlices();  //-- tprDebug 但是不在此文件中 clear
        //tprDebug::draw_renderPool_pointPics();     //-- tprDebug 但是不在此文件中 clear

    //-- opaque First, Translucent Second !!! --
    esrc::get_renderPool(RenderPoolType::Opaque).draw();
    esrc::get_renderPool(RenderPoolType::Translucent).draw();

    esrc::get_renderPool(RenderPoolType::Shadow).draw(); // 确保最后渲染
}



namespace sc_world_inn {//-------------- namespace: sc_world_inn ------------------//



void inputINS_handle( const InputINS &inputINS_){

    Player &playerRef = esrc::get_player();
    GameObj &playerGoRef = playerRef.get_goRef();
    //-----------------//
    //      camera
    //-----------------//
    //-- 让 camera 对其上1渲染帧 --
    //- 这会造成 camera 的延迟，但不要紧
    esrc::get_camera().set_targetDPos( playerGoRef.get_dpos() );

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


    if( inputINS_.get_key(GameKey::A) ){ isNew_A_press = true; }
    if( inputINS_.get_key(GameKey::B) ){ isNew_B_press = true; }
    if( inputINS_.get_key(GameKey::X) ){ isNew_X_press = true; }
    if( inputINS_.get_key(GameKey::Y) ){ isNew_Y_press = true; }


    SpeedLevel lvl = playerGoRef.move.moveSpeedLvl.get_newVal();
    //-- 有效的 节点帧 --
    if( (isOld_A_press==false) && (isNew_A_press) ){

        if( speedLevel_2_size_t(lvl) < speedLevel_2_size_t(SpeedLevel::LV_10) ){
            SpeedLevel newLvl = calc_higher_speedLvl(lvl);
            playerRef.set_moveSpeedLvl( newLvl );

                tprDebug::console( 
                    " + {0}, {1}",
                    static_cast<int>(newLvl),
                    SpeedLevel_2_val(newLvl)
                );


        }
    }
    if( (isOld_B_press==false) && (isNew_B_press) ){
        SpeedLevel newLvl = calc_lower_speedLvl(lvl);
        playerRef.set_moveSpeedLvl( newLvl );
        
            tprDebug::console( 
                " - {0}, {1}",
                static_cast<int>(newLvl) ,
                SpeedLevel_2_val(newLvl)
            );
    }
    if( (isOld_X_press==false) && (isNew_X_press) ){

        playerGoRef.debug();
        //...
        //innTest::innTest_main();
        
        
    }
    if( (isOld_Y_press==false) && (isNew_Y_press) ){
        //esrc::chunks_debug();

        esrc::debug_for_gameObjs();
        esrc::debug_for_renderPools();

    }

    //---------------
    isOld_A_press = isNew_A_press;
    isOld_B_press = isNew_B_press;
    isOld_X_press = isNew_X_press;
    isOld_Y_press = isNew_Y_press;
}


}//------------------ namespace: sc_world_inn end ------------------//


