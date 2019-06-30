/*
 * ========================= Lichen_DForest.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/mapSurfaces/Lichen_DForest.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 
#include "Script/gameObjs/create_go_oth.h"


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                 init_in_autoMod
 * -----------------------------------------------------------
 */
void Lichen_DForest::init_in_autoMod(  GameObj &goRef_,
                                const IntVec2 &mpos_,
					            float fieldWeight_,
					            const MapAltitude &alti_,
					            const Density &_density ){

    //-------- go.pvtBinary ---------//
    goRef_.resize_pvtBinary( sizeof(Lichen_DForest_PvtBinary) );
    Lichen_DForest_PvtBinary  *pvtBp = reinterpret_cast<Lichen_DForest_PvtBinary*>(goRef_.get_pvtBinaryPtr());

        pvtBp->lichen_DForestId = gameObjs::apply_a_simpleId( fieldWeight_, 24 );


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Lichen_DForest::OnRenderUpdate, &lichen_DForest, _1 );   
    goRef_.LogicUpdate  = std::bind( &Lichen_DForest::OnLogicUpdate,  &lichen_DForest, _1 );
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Lichen_DForest::OnActionSwitch, &lichen_DForest, _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Idle );
            //- 当前 lichen_DForest 只有一种动画，就是永久待机...

    //-------- go self vals ---------//
    goRef_.species = Lichen_DForest::specId;
    goRef_.family = GameObjFamily::Major;
    goRef_.parentId = NULLID;
    goRef_.state = GameObjState::Waked;
    goRef_.moveState = GameObjMoveState::AbsFixed; //- 无法移动
    goRef_.weight = 1.0f;

    goRef_.isTopGo = true;
    goRef_.isActive = true;
    goRef_.isDirty = false;
    goRef_.isControlByPlayer = false;

    goRef_.move.set_speedLvl( SpeedLevel::LV_0 );
    goRef_.move.set_MoveType( MoveType::Crawl );

    goRef_.set_collision_isDoPass( false );
    goRef_.set_collision_isBePass( true );  //- 碰撞区 可以被其它go 穿过

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goRef_.creat_new_goMesh("root", //- gmesh-name
                                        RenderLayerType::MapSurfaces, 
                                        &esrc::get_rect_shader(),  
                                        &esrc::get_rect_shader(), //- 其实没有 shadow
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        0.0,  //- off_z
                                        true, //- isVisible
                                        true, //- isCollide
                                        gameObjs::apply_isFlipOver( fieldWeight_ ) //- isFlipOver
                                        );

        rootGoMeshRef.bind_animAction( "lichen_DForest", 
                                        tprGeneral::nameString_combine( "", pvtBp->lichen_DForestId, "_idle" ) ); 

        goRef_.set_rootColliEntHeadPtr( &rootGoMeshRef.get_currentFramePos().get_colliEntHead() ); //- 先这么实现...         

    //-- 务必在 mesh:"root" 之后 ---
    goRef_.goPos.set_alti( 0.0f );
    goRef_.goPos.init_by_currentMPos( mpos_ );
    //...

    //-------- go.pubBinary ---------//
    goRef_.pubBinary.init( lichen_DForest_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Lichen_DForest::bind( GameObj &goRef_ ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Lichen_DForest::rebind( GameObj &goRef_ ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Lichen_DForest::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Lichen_DForest_PvtBinary  *pvtBp = this->rebind_ptr( goRef_ );

    //=====================================//
    //              AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    //goRef_.move.RenderUpdate();
            // 目前来看，永远也不会 移动...

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void Lichen_DForest::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Lichen_DForest_PvtBinary  *pvtBp = this->rebind_ptr( goRef_ );
    //=====================================//

    // 什么也没做...
}



/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void Lichen_DForest::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    Lichen_DForest_PvtBinary  *pvtBp = this->rebind_ptr( goRef_ );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    //GameObjMesh &rootGoMeshRef = goRef_.goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Move_Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            //rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->lichen_DForestId );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}


}//------------- namespace gameObjs: end ----------------

