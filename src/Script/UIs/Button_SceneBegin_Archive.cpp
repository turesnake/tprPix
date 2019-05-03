/*
 * =========== Button_SceneBegin_Archive.cpp ================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/UIs/Button_SceneBegin_Archive.h"

//-------------------- C --------------------//
#include <cassert> //- assert
#include <cmath>

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 
#include "Script/gameObjs/create_go_oth.h"

using namespace std::placeholders;

//#include "debug.h" 

namespace uis{//------------- namespace uis ----------------

/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::init_in_autoMod(  GameObj *_goPtr,
                                                const IntVec2 &_mpos ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &Button_SceneBegin_Archive::OnRenderUpdate, &button_sceneBegin_archive, _1 );   
    goPtr->LogicUpdate  = std::bind( &Button_SceneBegin_Archive::OnLogicUpdate,  &button_sceneBegin_archive, _1 );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &Button_SceneBegin_Archive::OnActionSwitch, &button_sceneBegin_archive, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
            //- 当前 只有一种动画，就是永久待机...

    //-------- go self vals ---------//
    goPtr->species = Button_SceneBegin_Archive::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 1.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_1 );   //- 此值暂时不影响 运动效果
    goPtr->move.set_MoveType( MoveType::TargetDrag );
    goPtr->move.set_targetDrag_targetFPos( mpos_2_fpos(_mpos) );

    goPtr->set_collision_isDoPass( false );
    goPtr->set_collision_isBePass( true );  //- 碰撞区 可以被其它go 穿过

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goPtr->creat_new_goMesh("root", //- gmesh-name
                                        "button_beginScene", //- animFrameSet-Name
                                        RenderLayerType::UIs, //- 固定zOff值 
                                        &esrc::rect_shader,  
                                        &esrc::rect_shader, //- 其实没有 shadow
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        1.1,  //- off_z 通过此值来确保，archive 在 pointer 后方。目前这个实现很不好...
                                        true, //- isVisible
                                        false, //- isCollide
                                        false //- isFlipOver
                                        );

        //-- bind animFrameSet / animFrameIdxHandle --
        rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( 1 ); //- "new"

    //-- 务必在 mesh:"root" 之后 ---
    goPtr->goPos.set_alti( 0.0f ); //- ui 中的此值 只能设置为 0
    goPtr->goPos.init_by_currentMPos( _mpos );

}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::OnRenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goPtr->move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    for( auto &pairRef : goPtr->goMeshs ){
        pairRef.second.RenderUpdate();
    }
}

/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::OnLogicUpdate( GameObj *_goPtr ){
    // 什么也没做...
}

/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void Button_SceneBegin_Archive::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){
    // 暂时什么也没做...
}

}//------------- namespace uis: end ----------------

