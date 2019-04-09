/*
 * ========================= BigMan.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/BigMan.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 

using namespace std::placeholders;

#include "debug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


namespace{//-------------- namespace ------------------//
}//------------------ namespace: end ------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void BigMan::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &BigMan::OnRenderUpdate, &big_man, _1 );   
    goPtr->LogicUpdate  = std::bind( &BigMan::OnLogicUpdate,  &big_man, _1 );

    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &BigMan::OnActionSwitch, &big_man, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Move );


    //-------- go self vals ---------//
    goPtr->species = BigMan::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 5.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_3 );
    goPtr->move.set_MoveType( true ); //- tmp

    goPtr->goPos.set_alti( 0.0f );

    goPtr->set_collision_isDoPass( false );
    goPtr->set_collision_isBePass( false );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &goMeshRef = goPtr->creat_new_goMesh( "root" );
        goMeshRef.init( goPtr ); 
        goMeshRef.picMesh.set_shader_program( &esrc::rect_shader );
        goMeshRef.shadowMesh.set_shader_program( &esrc::rect_shader );
        //-- bind animFrameSet / animFrameIdxHandle --
        goMeshRef.bind_animFrameSet( "bigMan" );
        goMeshRef.animFrameIdxHandle.bind_cycle(0,   //- 起始图元帧序号
                                                5,   //- 结束图元帧序号
                                                0,   //- 入口图元帧序号  
                                                true //- isOrder
                                                );
        
        goMeshRef.isVisible = true;
        goMeshRef.isCollide = true;
        goMeshRef.isFlipOver = false;

        //-- goMesh pos in go --
        goMeshRef.pposOff = glm::vec2{ 0.0f, 0.0f }; //- 此 goMesh 在 go 中的 坐标偏移 
        goMeshRef.off_z = 0.0f;  //- 作为 0号goMesh,此值必须为0

    //-------- go.binary ---------//
    goPtr->resize_pvtBinary( sizeof(BigMan_PvtBinary) );
    pvtBp = (BigMan_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针


    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( bigMan_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void BigMan::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void BigMan::rebind( GameObj *_goPtr ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void BigMan::OnRenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );

    //=====================================//
    //           test: AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goPtr->move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    for( auto &pairRef : goPtr->goMeshs ){
        GameObjMesh &goMeshRef = pairRef.second;

        //-- 也许不该放在 这个位置 --
        if( goMeshRef.isVisible == false ){
            continue;
        }

        goMeshRef.animFrameIdxHandle.update();

        goMeshRef.shadowMesh.refresh_translate();
        goMeshRef.shadowMesh.refresh_scale_auto(); //- 没必要每帧都执行
        goMeshRef.picMesh.refresh_translate();
        goMeshRef.picMesh.refresh_scale_auto(); //- 没必要每帧都执行

        esrc::renderPool_goMeshs_pic.insert({ goMeshRef.shadowMesh.get_render_z(), (ChildMesh*)&(goMeshRef.shadowMesh) });
        esrc::renderPool_goMeshs_shadow.push_back( (ChildMesh*)&(goMeshRef.picMesh) );
    }
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void BigMan::OnLogicUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void BigMan::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goPtr->goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( _type ){
        case ActionSwitchType::Move_Idle:
            //goMeshRef.bind_animFrameSet( "bigMan" );
            goMeshRef.animFrameIdxHandle.bind_cycle(0,   //- 起始图元帧序号
                                                    5,   //- 结束图元帧序号
                                                    0,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        case ActionSwitchType::Move_Move:
            //goMeshRef.bind_animFrameSet( "bigMan" );
            goMeshRef.animFrameIdxHandle.bind_cycle(6,   //- 起始图元帧序号
                                                    11,  //- 结束图元帧序号
                                                    6,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }


}


/* ===========================================================
 *                   create_a_BigMan
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_BigMan( const IntVec2 &_mpos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针
    gameObjs::big_man.init( goPtr );
    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );

        esrc::goids_active.insert( goid ); //- tmp

    return goid;
}


}//------------- namespace gameObjs: end ----------------

