/*
 * =================== sceneLoop_world.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   正式游戏的 sceneLoop
 *   目前，整个文件的 实现 都很潦草简陋...
 *  
 *   希望能在未来，实现 更加正式的 UI ／ scene 系统
 * 
 * ----------------------------
 */
#include "sceneLoop.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
//#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "input.h"
#include "IntVec.h"
#include "chunkBuild.h"
#include "esrc_all.h" //- 所有资源
#include "dataBase.h"
#include "GameArchive.h"
#include "GameSeed.h"
#include "esrc_gameSeed.h"
#include "esrc_gameArchive.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/create_UIs.h"
#include "Script/resource/ssrc.h"
#include "Script/gameObjs/create_goes.h"


using namespace std::placeholders;

#include "debug.h"


namespace{//-------------- namespace ------------------//

    //-- 三个按钮的 位置 --
    const std::vector<IntVec2> buttonMPoses {
        IntVec2 {0, 6},  //- 0
        IntVec2 {0, 0},  //- 1
        IntVec2 {0, -6}  //- 2
    };

    int targetIdx {0}; //- 用来指向 buttonMPoses : 0,1,2

    //- 一种简陋的方法，来降低 input 输入频率，
    //  每获得一次有效输入后，屏蔽之后10帧的 输入
    int  inputFrameCount {0};
    bool is_input_open {true};

    GameObj *button_archive_1_Ptr;
    GameObj *button_archive_2_Ptr;
    GameObj *button_archive_3_Ptr;
    GameObj *button_pointer_Ptr;


    std::unordered_map<gameArchiveId_t, GameArchive> gameArchives {};


    //===== funcs =====//
    void inputINS_handle_in_sceneBegin( const InputINS &_inputINS);

}//------------------ namespace: end ------------------//


/* ===========================================================
 *                prepare_for_sceneBegin
 * -----------------------------------------------------------
 * -- 在 siwth 进入 scene:begin 之前，需要准备的工作
 */
void prepare_for_sceneBegin(){

    goid_t pointerId = uis::create_a_ui( ssrc::get_uiSpecId("button_sceneBegin_pointer"), 
                                            buttonMPoses.at(0) );

    goid_t archiveId_1 = uis::create_a_ui( ssrc::get_uiSpecId("button_sceneBegin_archive"), 
                                            buttonMPoses.at(0) );

    goid_t archiveId_2 = uis::create_a_ui( ssrc::get_uiSpecId("button_sceneBegin_archive"), 
                                            buttonMPoses.at(1) );

    goid_t archiveId_3 = uis::create_a_ui( ssrc::get_uiSpecId("button_sceneBegin_archive"), 
                                            buttonMPoses.at(2) );


    button_pointer_Ptr   = esrc::get_memUIPtr( pointerId );
    button_archive_1_Ptr = esrc::get_memUIPtr( archiveId_1 );
    button_archive_2_Ptr = esrc::get_memUIPtr( archiveId_2 );
    button_archive_3_Ptr = esrc::get_memUIPtr( archiveId_3 );


    //----------------------------//
    //            db
    //----------------------------//

    //-- 从 数据库读取 所有 gameArchives 的数据 --
    db::atom_select_all_from_table_gameArchive( gameArchives );
        assert( gameArchives.size() <= 3 );
    
    for( const auto &pair : gameArchives ){

        //-- 如果哪个 存档已经有数据了，修改其图标（显示"data"）
        //   简单粗暴的实现，临时...
        switch( pair.first ){
            case 1:  button_archive_1_Ptr->goMeshs.at("root").getnc_animFrameIdxHandle().bind_idle( 2 ); break;
            case 2:  button_archive_2_Ptr->goMeshs.at("root").getnc_animFrameIdxHandle().bind_idle( 2 ); break;
            case 3:  button_archive_3_Ptr->goMeshs.at("root").getnc_animFrameIdxHandle().bind_idle( 2 ); break;
            default:
                assert(0);
        }
    }
    
    
    input::bind_inputINS_handleFunc( std::bind( &inputINS_handle_in_sceneBegin, _1 ) );
    // 可能要 关闭 camera

    switch_sceneLoop( SceneLoopType::Begin );
}



/* ===========================================================
 *                    sceneLoop_begin
 * -----------------------------------------------------------
 * -- 游戏初始界面  
 *    最最简单的样式，快速填补功能空白。在未来，需要重新设计
 */
void sceneLoop_begin(){

    //--------------------------------//
    //    camera:: RenderUpdate()
    //    camera --> shader: view, projection
    //--------------------------------//
    
    esrc::camera.RenderUpdate();
    //--- 
    esrc::rect_shader.use_program();
    esrc::rect_shader.send_mat4_view_2_shader( esrc::camera.update_mat4_view() );
    esrc::rect_shader.send_mat4_projection_2_shader( esrc::camera.update_mat4_projection() );
    

    //====================================//
    //          -- RENDER --
    //    Z-Deep 深的 mesh 必须先渲染
    //====================================//

    //--- clear RenderPools:
    // *** 注意次序 ***
    esrc::renderPool_uiMeshs_pic.clear();

    //------------------------//
    //     - shadowMeshs
    //     - picMeshs
    //------------------------//
    esrc::foreach_uiIds_active(
        []( goid_t _goid, GameObj *_goPtr ){
            assert( _goPtr->RenderUpdate != nullptr );
            _goPtr->RenderUpdate( _goPtr ); 
        }
    );


    //>>>>>>>>>>>>>>>>>>>>>>>>//
    //        draw call
    //>>>>>>>>>>>>>>>>>>>>>>>>//
    esrc::draw_renderPool_uiMeshs_pic(); 

}

namespace{//-------------- namespace ------------------//


/* ===========================================================
 *              inputINS_handle_in_sceneBegin
 * -----------------------------------------------------------
 * -- 目前未实现对 ESC 健的处理...
 */
void inputINS_handle_in_sceneBegin( const InputINS &_inputINS){


    //-- 当获得一次有效 input后，屏蔽之后的 10 帧。
    if( is_input_open == false ){
        inputFrameCount++;
        if( inputFrameCount < 10 ){
            return;
        }else{
            is_input_open = true;
        }
    }

    //-----------------------//
    //      Key: ENTER
    //-----------------------//
    if( _inputINS.check_key(GameKey::KEY_10) ){
        is_input_open = false;
        cout << "enter" << endl;

        //-- 此时，targetIdx 就是选中的 archive
        //  可以将其 写入 游戏mem态，并进入 后续流程
        //  创建／读取 相关 数据库 table。结束 scenebegin，准备进入 sceneWorld

        gameArchiveId_t archiveId = targetIdx+1;

        if( gameArchives.find(archiveId) == gameArchives.end() ){
            //-- 玩家选中的 存档为 空 --
            u32_t target_baseSeed = GameSeed::apply_new_baseSeed();
            esrc::gameSeed.init( target_baseSeed );

            //-- max goid --
            // 必须在 chunks／goes 生成之前
            u64_t   maxGoId = 1;
            GameObj::id_manager.set_max_id(maxGoId);

                //-- 随便定个 mpos 
                IntVec2    newGoMPos { 8,0 };
                //--- 先 生成 chunks 基础数据 --
                chunkBuild::build_9_chunks( newGoMPos );
                        //-- 在未来，需要读取 db::table_chunks 的数据，来辅助生成 chunks
                        //   这部分，应该写进 chunk build 流程中 ...

                //-- db::table_goes --
                goSpecId_t newGoSpecId = ssrc::get_goSpecId( "norman" );
                goid_t newGoId = gameObjs::create_a_Go(   newGoSpecId,
                                                            newGoMPos,
                                                            0.0,
                                                            MapAltitude {},
                                                            Density {} );
                db::atom_insert_or_replace_to_table_goes( DiskGameObj{ newGoId, newGoSpecId, newGoMPos } );
                //-- db::table_gameArchive --
                
                
                esrc::gameArchive = GameArchive {   archiveId, 
                                                    target_baseSeed,
                                                    newGoId,
                                                    newGoMPos,
                                                    GameObj::id_manager.get_max_id() //- chunk 生成后，maxId 变了
                                                    };
                db::atom_insert_or_replace_to_table_gameArchive( esrc::gameArchive );

                //-- player --
                esrc::player.bind_go( newGoId ); //-- 务必在 go数据实例化后 再调用 --

        }else{
            //-- 玩家选中的 存档 已经存在 --
            esrc::gameArchive = gameArchives.at(archiveId); //- copy

            esrc::gameSeed.init( esrc::gameArchive.baseSeed );
            GameObj::id_manager.set_max_id( esrc::gameArchive.maxGoId );

                cout << "id_manager.get_max_id() = " << GameObj::id_manager.get_max_id() << endl;

            //-- db::table_goes --
            DiskGameObj diskGo {};
            db::atom_select_one_from_table_goes( esrc::gameArchive.playerGoId, diskGo );


                assert( diskGo.mpos == esrc::gameArchive.playerGoMPos ); //- tmp
            
            //--- 先生成 chunks --
            chunkBuild::build_9_chunks( esrc::gameArchive.playerGoMPos );
            
                //... 根据 读取的数据，将其转换为 mem go 实例 ...
            gameObjs::rebind_a_disk_Go( diskGo,
                                        0.0,
                                        MapAltitude {},
                                        Density {} );

            //-- player --
            esrc::player.bind_go( esrc::gameArchive.playerGoId ); //-- 务必在 go数据实例化后 再调用 --
        }


        // 准备进入 sceneWorld ...
        prepare_for_sceneWorld();
        return;
    }


    //-- 只检查 上下 健 --
    if( _inputINS.check_key(GameKey::UP) ){
        is_input_open = false;
        //---
        targetIdx--;
        if( targetIdx < 0 ){
            targetIdx = buttonMPoses.size() - 1;
        }
        button_pointer_Ptr->move.set_targetDrag_targetFPos( mpos_2_fpos( buttonMPoses.at(targetIdx) ) );

    }else if( _inputINS.check_key(GameKey::DOWN) ){
        is_input_open = false;  
        //---
        targetIdx++;
        if( targetIdx >= buttonMPoses.size() ){
            targetIdx = 0;
        }
        button_pointer_Ptr->move.set_targetDrag_targetFPos( mpos_2_fpos( buttonMPoses.at(targetIdx) ) );
    }


    if( is_input_open == false ){
        inputFrameCount = 0; //- clear 0
    }

}


}//------------------ namespace: end ------------------//

