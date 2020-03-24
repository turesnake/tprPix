/*
 * =================== begin_scene.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   begin: scene
 * 
 *   目前，整个文件的 实现 都很潦草简陋...
 *  
 *   希望能在未来，实现 更加正式的 UI ／ scene 系统
 * 
 * ----------------------------
 */
#include "pch.h"
#include "sceneLoop.h"

//-------------------- Engine --------------------//
#include "input.h"
#include "chunkCreate.h"
#include "dataBase.h"
#include "GameArchive.h"
#include "GameSeed.h"
#include "UIAnchor.h"
#include "create_goes.h"

#include "GoSpecFromJson.h"
#include "UIGoSpecFromJson.h"


#include "ubo_all.h"

#include "esrc_all.h" //- 所有资源



using namespace std::placeholders;


namespace sc_begin_inn {//-------------- namespace: sc_begin_inn ------------------//

    //-- 三个按钮的 位置 --
    const std::vector<UIAnchor> butonUIAnchors {
        UIAnchor { glm::dvec2 { 0.0, 0.0 },    // center of window
                   glm::dvec2 { 0.0, 64.0 } },  //- offDPos

        UIAnchor { glm::dvec2 { 0.0, 0.0 },    // center of window
                   glm::dvec2 { 0.0, 0.0 } },  //- offDPos

        UIAnchor { glm::dvec2 { 0.0, 0.0 },    // center of window
                   glm::dvec2 { 0.0, -64.0 } },  //- offDPos
    };

    size_t targetIdx {0}; //- 用来指向 buttonMPoses : {0,1,2}

    //- 一种简陋的方法，来降低 input 输入频率，
    //  每获得一次有效输入后，屏蔽之后10帧的 输入
    int  inputFrameCount {0};
    int  inputGap        {10}; //- 间隙
    bool is_input_open   {true};

    goid_t button_archiveId_1 {};
    goid_t button_archiveId_2 {};
    goid_t button_archiveId_3 {};
    goid_t button_pointerId   {};


    std::unordered_map<gameArchiveId_t, GameArchive> gameArchives {};


    //===== funcs =====//
    void inputINS_handle( const InputINS &inputINS_);

    // tmp
    GameObj &get_goRef( goid_t id_ )noexcept{
        auto goOpt = esrc::get_goPtr( id_ );
        tprAssert( goOpt.has_value() );
        return *goOpt.value();
    }

}//------------------ namespace: sc_begin_inn end ------------------//



/* 在 进入 scene:begin 之前，需要准备的工作
 */
void prepareForScene_begin(){

    // input init 
    input::InputInit_for_begin(); // 从 main.cpp 转移来的

    //----------------------------//
    //      create ui objs
    //----------------------------//
    //- 所有 uigo，默认使用 1m1 collientset, 手动修正，使得 rootanchor，对齐于 目标 mpos
    //  最简陋的实现，tmp...
    IntVec2 initPPosOff { -HALF_PIXES_PER_MAPENT<>, -HALF_PIXES_PER_MAPENT<> };

    sc_begin_inn::button_pointerId = uiGos::create_a_UIGo(UIGoSpecFromJson::str_2_uiGoSpeciesId("button_sceneBegin_pointer_2"), 
                                                            sc_begin_inn::butonUIAnchors.at(0),
                                                            emptyDyParam,
                                                            17 ); // tmp，随便写的 goUWeight
    //---
    sc_begin_inn::button_archiveId_1 = uiGos::create_a_UIGo(UIGoSpecFromJson::str_2_uiGoSpeciesId("button_sceneBegin_archive_2"), 
                                                            sc_begin_inn::butonUIAnchors.at(0),
                                                            emptyDyParam,
                                                            17 ); // tmp，随便写的 goUWeight

    sc_begin_inn::button_archiveId_2 = uiGos::create_a_UIGo(UIGoSpecFromJson::str_2_uiGoSpeciesId("button_sceneBegin_archive_2"), 
                                                            sc_begin_inn::butonUIAnchors.at(1),
                                                            emptyDyParam,
                                                            17 ); // tmp，随便写的 goUWeight
    
    sc_begin_inn::button_archiveId_3 = uiGos::create_a_UIGo(UIGoSpecFromJson::str_2_uiGoSpeciesId("button_sceneBegin_archive_2"), 
                                                            sc_begin_inn::butonUIAnchors.at(2),
                                                            emptyDyParam,
                                                            17 ); // tmp，随便写的 goUWeight

    GameObj &button_pointerRef = sc_begin_inn::get_goRef( sc_begin_inn::button_pointerId );
    GameObj &button_archiveRef_1 = sc_begin_inn::get_goRef( sc_begin_inn::button_archiveId_1 );
    GameObj &button_archiveRef_2 = sc_begin_inn::get_goRef( sc_begin_inn::button_archiveId_2 );
    GameObj &button_archiveRef_3 = sc_begin_inn::get_goRef( sc_begin_inn::button_archiveId_3 );

    //----------------------------//
    //            db
    //----------------------------//
    //-- 从 数据库读取 所有 gameArchives 的数据 --
    db::atom_select_all_from_table_gameArchive( sc_begin_inn::gameArchives );
        tprAssert( sc_begin_inn::gameArchives.size() <= 3 );
    
    for( const auto &pair : sc_begin_inn::gameArchives ){
        //-- 如果哪个 存档已经有数据了，修改其图标（显示"data"）
        //   简单粗暴的实现，临时...
        switch( pair.first ){
            case 1:  
                button_archiveRef_1.actionSwitch.call_func(ActionSwitchType::ButtonState_2);
                break;
            case 2:  
                button_archiveRef_2.actionSwitch.call_func(ActionSwitchType::ButtonState_2);
                break;
            case 3:  
                button_archiveRef_3.actionSwitch.call_func(ActionSwitchType::ButtonState_2);
                break;
            default:
                tprAssert(0);
        }
    }
    
    input::bind_inputINS_handleFunc( std::bind( &sc_begin_inn::inputINS_handle, _1 ) );
    switch_sceneLoop( SceneLoopType::Begin );

}




void sceneLogicLoop_begin(){
    //  nothing ...
}



void sceneRenderLoop_begin(){

    //--------------------------------//
    //    camera:: RenderUpdate()
    //--------------------------------//
    //-- 此时 方向键 并不影响 camera 位移
    esrc::get_camera().RenderUpdate();

    //--------------------------------//
    //             ubo
    //--------------------------------//
    ubo::write_ubo_Camera(); 
    ubo::write_ubo_Time();

    //--------------------------------//
    //     clear RenderPools
    //--------------------------------//
    esrc::clear_all_renderPool();

    //------------------------//
    //     - shadowMeshs
    //     - picMeshs
    //------------------------//
    GameObj &button_pointerRef   = sc_begin_inn::get_goRef( sc_begin_inn::button_pointerId );
    GameObj &button_archiveRef_1 = sc_begin_inn::get_goRef( sc_begin_inn::button_archiveId_1 );
    GameObj &button_archiveRef_2 = sc_begin_inn::get_goRef( sc_begin_inn::button_archiveId_2 );
    GameObj &button_archiveRef_3 = sc_begin_inn::get_goRef( sc_begin_inn::button_archiveId_3 );

    button_pointerRef.RenderUpdate( button_pointerRef );
    button_archiveRef_1.RenderUpdate( button_archiveRef_1 );
    button_archiveRef_2.RenderUpdate( button_archiveRef_2 );
    button_archiveRef_3.RenderUpdate( button_archiveRef_3 );
                        
    //>>>>>>>>>>>>>>>>>>>>>>>>//
    //        draw call
    //>>>>>>>>>>>>>>>>>>>>>>>>//
    //-- opaque First, Translucent Second !!! --
    esrc::get_renderPool(RenderPoolType::Opaque).draw();
    esrc::get_renderPool(RenderPoolType::Translucent).draw();

}


namespace sc_begin_inn {//-------------- namespace: sc_begin_inn ------------------//



/* 目前未实现对 ESC 健的处理...
 */
void inputINS_handle( const InputINS &inputINS_){

    //-- 当获得一次有效 input后，屏蔽之后的 10 帧。
    if( is_input_open == false ){
        inputFrameCount++;
        if( inputFrameCount < sc_begin_inn::inputGap ){
            return;
        }else{
            is_input_open = true;
        }
    }

    //-----------------------//
    //      Key: H 
    //      Key: ENTER
    //-----------------------//
    if( inputINS_.get_key(GameKey::A) ){
        is_input_open = false;
        tprDebug::console( "enter" );

        //-- 此时，targetIdx 就是选中的 archive
        //  可以将其 写入 游戏mem态，并进入 后续流程
        //  创建／读取 相关 数据库 table。结束 scenebegin，准备进入 sceneWorld

        gameArchiveId_t archiveId = static_cast<gameArchiveId_t>(targetIdx+1);// 很粗暴的方式

        if( gameArchives.find(archiveId) == gameArchives.end() ){

            tprDebug::console( "New Archive" );
            //-----------------------//
            //   玩家选中的 存档为 空 
            //-----------------------//
            uint32_t target_baseSeed = GameSeed::apply_new_baseSeed();
            esrc::get_gameSeed().init( target_baseSeed );

            //-- gameTime --
            double newGameTime { 8.0 };
            esrc::get_timer().start_record_gameTime( newGameTime );

            //-- max goid --
            // 新存档无需重制 goMaxId, 因为在创建本 scene 时，uigo 已经占用了 goid号
            // tmp...

                //-- 随便定个 mpos 
                IntVec2    newGoMPos    { 0,0 };
                glm::dvec2 newGoDPos = mpos_2_dpos( newGoMPos );

                //tprDebug::console( "9-chunks create start" );

                //--- 先 生成 chunks 基础数据 --
                chunkCreate::create_9_chunks( newGoMPos );
                        //-- 在未来，需要读取 db::table_chunks 的数据，来辅助生成 chunks
                        //   这部分，应该写进 chunk build 流程中 ...



                //-- db::table_goes --

                //----- 临时创建，玩家操纵的 go -------//
                // old
                //tprDebug::console( "chicken create start" );

                goSpeciesId_t newGoSpeciesId = GoSpecFromJson::str_2_goSpeciesId( "chicken" );
                goLabelId_t   newGoLabelId = GoAssemblePlanSet::str_2_goLabelId("Hen");

                auto goDataUPtr = GoDataForCreate::create_new_goDataForCreate(  
                                                    newGoMPos,
                                                    newGoDPos,
                                                    newGoSpeciesId,
                                                    newGoLabelId,
                                                    NineDirection::Left,
                                                    BrokenLvl::Lvl_0
                                                    );
                goid_t newGoId = gameObjs::create_a_Go( goDataUPtr.get() );


                db::atom_insert_or_replace_to_table_goes( DiskGameObj{  newGoId, 
                                                                        newGoSpeciesId,   
                                                                        newGoLabelId,
                                                                        newGoDPos,
                                                                        goDataUPtr->get_goUWeight(),
                                                                        NineDirection::Left,
                                                                        BrokenLvl::Lvl_0
                                                                        } );              



                //-- db::table_gameArchive --                
                esrc::get_gameArchive() = GameArchive {   archiveId, 
                                                    target_baseSeed,
                                                    newGoId,
                                                    newGoDPos,
                                                    GameObj::id_manager.get_max_id(), //- chunk 生成后，maxId 变了
                                                    newGameTime  
                                                    };
                db::atom_insert_or_replace_to_table_gameArchive( esrc::get_gameArchive() );

                //-- player --
                esrc::get_player().bind_go( newGoId ); //-- 务必在 go数据实例化后 再调用 --

        }else{
            tprDebug::console( "Exist Archive" );
            //-----------------------//
            //  玩家选中的 存档 已经存在 
            //-----------------------//
            GameArchive &targetGameArchive = esrc::get_gameArchive();

            targetGameArchive = gameArchives.at(archiveId); //- copy

            esrc::get_gameSeed().init( targetGameArchive.baseSeed );
            GameObj::id_manager.set_max_id( targetGameArchive.maxGoId );

                tprDebug::console(  "maxGoId_from_db = {}", GameObj::id_manager.get_max_id() );

            //-- gameTime --
            esrc::get_timer().start_record_gameTime( targetGameArchive.gameTime );

                tprDebug::console( "gameTime_from_db = {}", targetGameArchive.gameTime );

            //-- db::table_goes --
            DiskGameObj diskGo {};
            db::atom_select_one_from_table_goes( targetGameArchive.playerGoId, diskGo );

                tprAssert( glm::length(diskGo.dpos - targetGameArchive.playerGoDPos) < 1.0 ); //- tmp
            
            //--- 先生成 chunks --
            chunkCreate::create_9_chunks( dpos_2_mpos( targetGameArchive.playerGoDPos ) );
            
            //  重建 playerGo 实例：
            //... 根据 读取的数据，将其转换为 mem go 实例 ...

            gameObjs::rebind_a_diskGo( diskGo );


            //-- player --
            esrc::get_player().bind_go( targetGameArchive.playerGoId ); //-- 务必在 go数据实例化后 再调用 --
        }

        // 准备进入 sceneWorld ...
        prepareForScene_world();
        return;
    }


    //-- 只检查 上下 健 --

    if( inputINS_.is_dir_up() || inputINS_.get_key(GameKey::Hat_Up) ){ 

        is_input_open = false;
        //---
        if( targetIdx <= 0 ){
            targetIdx = butonUIAnchors.size()-1;
        }else{
            targetIdx--;
        }   
        //---    

        // get go ref
        auto goOpt = esrc::get_goPtr( sc_begin_inn::button_pointerId );
        tprAssert( goOpt.has_value() );
        GameObj &button_pointerRef = *goOpt.value();

        button_pointerRef.move.set_adsorb_targetDPos( butonUIAnchors.at(targetIdx).get_dpos() );


    }
    else if( inputINS_.is_dir_down() || inputINS_.get_key(GameKey::Hat_Down) ){

        is_input_open = false;  
        //---
        if( targetIdx >= butonUIAnchors.size()-1 ){
            targetIdx = 0;
        }else{
            targetIdx++;
        }
        //---

        // get go ref
        auto goOpt = esrc::get_goPtr( sc_begin_inn::button_pointerId );
        tprAssert( goOpt.has_value() );
        GameObj &button_pointerRef = *goOpt.value();

        button_pointerRef.move.set_adsorb_targetDPos( butonUIAnchors.at(targetIdx).get_dpos() );
    }

    if( is_input_open == false ){
        inputFrameCount = 0; //- clear 0
    }

}


}//------------------ namespace: sc_begin_inn end ------------------//

