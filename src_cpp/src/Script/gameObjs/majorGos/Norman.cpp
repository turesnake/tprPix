/*
 * ========================= Norman.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/Norman.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 


//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 * -- 最后三个参数 并未用上
 */
void Norman::init_in_autoMod(   GameObj &goRef_,
                                const IntVec2 &mpos_,
					            float fieldWeight_,
					            const MapAltitude &alti_,
					            const Density &_density ){

    //-------- go.pvtBinary ---------//
    goRef_.resize_pvtBinary( sizeof(Norman_PvtBinary) );
    Norman_PvtBinary *pvtBp = reinterpret_cast<Norman_PvtBinary*>(goRef_.get_pvtBinaryPtr()); //- 绑定到本地指针


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Norman::OnRenderUpdate, &norman, _1 );   
    goRef_.LogicUpdate  = std::bind( &Norman::OnLogicUpdate,  &norman, _1 );
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Norman::OnActionSwitch, &norman, _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Move );


    //-------- go self vals ---------//
    goRef_.species = Norman::specId;
    goRef_.family = GameObjFamily::Major;
    goRef_.parentId = NULLID;
    goRef_.state = GameObjState::Waked;
    goRef_.moveState = GameObjMoveState::Movable;
    goRef_.weight = 5.0f;

    goRef_.isTopGo = true;
    goRef_.isActive = true;
    goRef_.isDirty = false;
    goRef_.isControlByPlayer = false;

    //goPtr->move.set_speedLvl( SpeedLevel::LV_6 ); //- 标准crawl速度 4/5/6 都不错
    goRef_.move.set_speedLvl( SpeedLevel::LV_6 );   //- tmp，用来快速检索地图
    goRef_.move.set_MoveType( MoveType::Crawl );

    goRef_.set_collision_isDoPass( false );
    goRef_.set_collision_isBePass( false );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goRef_.creat_new_goMesh("root", //- gmesh-name
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        &esrc::get_rect_shader(),  
                                        &esrc::get_rect_shader(),
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        0.0,  //- off_z
                                        true, //- isVisible
                                        true, //- isCollide
                                        false //- isFlipOver
                                        );

        rootGoMeshRef.bind_animAction( "norman", "move_idle" );

        goRef_.set_rootColliEntHeadPtr( &rootGoMeshRef.get_currentFramePos().get_colliEntHead() ); //- 先这么实现...


    //-- 务必在 mesh:"root" 之后 ---
    goRef_.goPos.set_alti( 0.0f );
    goRef_.goPos.init_by_currentMPos( mpos_ );

    //...

    //-------- go.pubBinary ---------//
    goRef_.pubBinary.init( norman_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Norman::bind( GameObj &goRef_ ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Norman::rebind( GameObj &goRef_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Norman_PvtBinary *pvtBp = rebind_ptr( goRef_ );


}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Norman::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Norman_PvtBinary *pvtBp = rebind_ptr( goRef_ );

    //=====================================//
    //            AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    for( auto &pairRef : goRef_.goMeshs ){
        pairRef.second.RenderUpdate();
    }
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void Norman::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Norman_PvtBinary *pvtBp = rebind_ptr( goRef_ );
    //=====================================//

    // 什么也没做...
}



/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void Norman::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Norman_PvtBinary *pvtBp = rebind_ptr( goRef_ );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &rootGoMeshRef = goRef_.goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Move_Idle:
            rootGoMeshRef.bind_animAction( "norman", "move_idle" );


            break;

        case ActionSwitchType::Move_Move:
            rootGoMeshRef.bind_animAction( "norman", "move_walk" );

            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }


}

}//------------- namespace gameObjs: end ----------------

