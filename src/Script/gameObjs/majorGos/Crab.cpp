/*
 * ========================= Norman.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/Crab.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- tpr --------------------//
#include "tprFileSys.h"

//-------------------- Engine --------------------//
#include "esrc_shader.h" 


//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

using namespace std::placeholders;

#include "debug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


//namespace{//-------------- namespace ------------------//
//}//------------------ namespace: end ------------------//


/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 * -- 最后三个参数 并未用上
 */
void Crab::init_in_autoMod( GameObj *_goPtr,
                            const IntVec2 &_mpos,
					        float _fieldWeight,
					        const MapAltitude &_alti,
					        const Density &_density ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- go.pvtBinary ---------//
    goPtr->resize_pvtBinary( sizeof(Crab_PvtBinary) );
    pvtBp = (Crab_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针

    pvtBp->tmpVal = 999;

    pvtBp->ai_crab.init( _goPtr );
    

    pvtBp->ai_crab.bind_get_tmpVal_functor(
        [_goPtr](){
            Crab_PvtBinary *pvtBp = (Crab_PvtBinary*)_goPtr->get_pvtBinaryPtr();
            return pvtBp->tmpVal;
        }
    );


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &Crab::OnRenderUpdate, &crab, _goPtr );   
    goPtr->LogicUpdate  = std::bind( &Crab::OnLogicUpdate,  &crab, _goPtr );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &Crab::OnActionSwitch, &crab, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Move );


    //-------- go self vals ---------//
    goPtr->species = Crab::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 5.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    //goPtr->move.set_speedLvl( SpeedLevel::LV_6 ); //- 标准crawl速度 4/5/6 都不错
    goPtr->move.set_speedLvl( SpeedLevel::LV_5 );   //- tmp，用来快速检索地图
    goPtr->move.set_MoveType( MoveType::Crawl );

    goPtr->set_collision_isDoPass( false );
    goPtr->set_collision_isBePass( false );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goPtr->creat_new_goMesh("root", //- gmesh-name
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        &esrc::rect_shader,  
                                        &esrc::rect_shader,
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        0.0,  //- off_z
                                        true, //- isVisible
                                        true, //- isCollide
                                        false //- isFlipOver
                                        );
        rootGoMeshRef.bind_animAction( "crab", "appear" );

        goPtr->set_rootColliEntHeadPtr( &rootGoMeshRef.get_currentFramePos().get_colliEntHead() ); //- 先这么实现...


    //-- 务必在 mesh:"root" 之后 ---
    goPtr->goPos.set_alti( 0.0f );
    goPtr->goPos.init_by_currentMPos( _mpos );

    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( crab_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Crab::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Crab::rebind( GameObj *_goPtr ){
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Crab::OnRenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );

    //=====================================//
    //            AI
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
        pairRef.second.RenderUpdate();
    }
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void Crab::OnLogicUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    this->pvtBp->ai_crab.logicUpdate();

    // 什么也没做...
}



/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void Crab::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &rootGoMeshRef = goPtr->goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( _type ){
        case ActionSwitchType::Move_Idle:
            rootGoMeshRef.bind_animAction( "crab", "half_disappear" );


            break;

        case ActionSwitchType::Move_Move:
            rootGoMeshRef.bind_animAction( "crab", "move_walk" );

            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}


}//------------- namespace gameObjs: end ----------------

