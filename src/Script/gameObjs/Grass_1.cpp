/*
 * ========================= Grass_1.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   草 test
 * 
 * ----------------------------
 */
#include "Script/gameObjs/Grass_1.h"

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
void Grass_1::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &Grass_1::OnRenderUpdate, &grass_1, _1 );   
    goPtr->LogicUpdate  = std::bind( &Grass_1::OnLogicUpdate,  &grass_1, _1 );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &Grass_1::OnActionSwitch, &grass_1, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goPtr->actionSwitch.signUp( ActionSwitchType::BeCollide_From_Left );
    goPtr->actionSwitch.signUp( ActionSwitchType::BeCollide_From_Right );


    //-------- go self vals ---------//
    goPtr->species = Grass_1::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::AbsFixed;
    goPtr->weight = 100.0f; //- 不该被推动

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_6 ); //- 不该有速度
    goPtr->move.set_MoveType( true ); //- tmp 不该被移动

    goPtr->goPos.set_alti( 0.0f );

    goPtr->set_collision_isDoPass( true );
    goPtr->set_collision_isBePass( true );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &goMeshRef = goPtr->creat_new_goMesh( "root" );
        goMeshRef.init( goPtr ); 
        goMeshRef.picMesh.set_shader_program( &esrc::rect_shader );
        goMeshRef.shadowMesh.set_shader_program( &esrc::rect_shader );
        //-- bind animFrameSet / animFrameIdxHandle --
        goMeshRef.bind_animFrameSet( "grass_1" );
        goMeshRef.animFrameIdxHandle.bind_cycle(0,   //- 起始图元帧序号
                                                1,   //- 结束图元帧序号
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
    goPtr->resize_pvtBinary( sizeof(Grass_1_PvtBinary) );
    pvtBp = (Grass_1_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针

    
    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( grass_1_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Grass_1::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Grass_1::rebind( GameObj *_goPtr ){
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Grass_1::OnRenderUpdate( GameObj *_goPtr ){
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
    //goPtr->move.RenderUpdate();
        //- 草永远不会移动...

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    //auto ipair = goPtr->goMeshs.begin();
    //for( ; ipair!=goPtr->goMeshs.end(); ipair++ ){
    for( auto &ipair : goPtr->goMeshs ){
        GameObjMesh &goMeshRef = ipair.second;

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
void Grass_1::OnLogicUpdate( GameObj *_goPtr ){
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
void Grass_1::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

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
            //goMeshRef.bind_animFrameSet( "grass_1" );
            goMeshRef.animFrameIdxHandle.bind_cycle(0,   //- 起始图元帧序号
                                                    1,   //- 结束图元帧序号
                                                    0,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        case ActionSwitchType::BeCollide_From_Left:
            //goMeshRef.bind_animFrameSet( "grass_1" );
            goMeshRef.animFrameIdxHandle.bind_cycle(6,   //- 起始图元帧序号
                                                    11,   //- 结束图元帧序号
                                                    6,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        case ActionSwitchType::BeCollide_From_Right:
            //goMeshRef.bind_animFrameSet( "grass_1" );
            goMeshRef.animFrameIdxHandle.bind_cycle(0,   //- 起始图元帧序号
                                                    5,   //- 结束图元帧序号
                                                    0,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }

}



}//------------- namespace gameObjs: end ----------------

