/*
 * ========================= OakTree.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/trees/OakTree.h"

//-------------------- C --------------------//
//#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- tpr --------------------//
//#include "tprFileSys.h"
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

namespace{//-------------- namespace ------------------//

    //--- 将所有 oakId 分类，方便分配 ---
    std::vector<int> multiBranch_age1   { 0 };
    std::vector<int> singleTrunk_age1   { 1, 2 };
    //---
    std::vector<int> multiBranch_age2   { 3, 4 };
    std::vector<int> singleTrunk_age2   { 5, 6 };
    //---
    std::vector<int> multiBranch_age3   { 7, 8, 9, 10, 11 };
    std::vector<int> singleTrunk_age3   { 12, 13, 14, 15, 16 };
    //---
    std::vector<int> multiBranch_age4   { 16 }; //- 暂无
    std::vector<int> singleTrunk_age4   { 16 }; //- 暂无


    //===== funcs =====//
    int apply_a_oakId( int _age, float _fieldWeight, bool _isSingleTrunk );


}//------------------ namespace: end ------------------//


/* ===========================================================
 *                 init_in_autoMod
 * -----------------------------------------------------------
 */
void OakTree::init_in_autoMod(  GameObj *_goPtr,
                                const IntVec2 &_mpos,
					            float _fieldWeight,
					            const MapAltitude &_alti,
					            const Density &_density ){

    tprAssert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- go.pvtBinary ---------//
    goPtr->resize_pvtBinary( sizeof(OakTree_PvtBinary) );
    pvtBp = (OakTree_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针

        pvtBp->age = gameObjs::apply_treeAge_by_density( _density );
        pvtBp->isSingleTRunk = gameObjs::apply_isSingleTRunk( _fieldWeight );
        pvtBp->oakId = apply_a_oakId( pvtBp->age, _fieldWeight, pvtBp->isSingleTRunk );
        //...
        

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &OakTree::OnRenderUpdate, &oakTree, _goPtr );   
    goPtr->LogicUpdate  = std::bind( &OakTree::OnLogicUpdate,  &oakTree, _goPtr );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &OakTree::OnActionSwitch, &oakTree, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
                //- 当前树木只有一种动画，就是永久待机...

    //-------- go self vals ---------//
    goPtr->species = OakTree::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::AbsFixed; //- 无法移动
    goPtr->weight = 50.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLvl( SpeedLevel::LV_0 );
    goPtr->move.set_MoveType( MoveType::Crawl );

    goPtr->set_collision_isDoPass( false );
    //--- 小树，中树 可以被其它go 穿过，成年树不行 ---
    if( pvtBp->age <= 2 ){
        goPtr->set_collision_isBePass( true );
    }else{
        goPtr->set_collision_isBePass( false );
    }
    
    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = 
                goPtr->creat_new_goMesh("root", //- gmesh-name
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        &esrc::get_rect_shader(),  
                                        &esrc::get_rect_shader(),
                                        glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                        0.0,  //- off_z
                                        true, //- isVisible
                                        true, //- isCollide
                                        gameObjs::apply_isFlipOver( _fieldWeight ) //- isFlipOver
                                        );

        rootGoMeshRef.bind_animAction( "oakTree", 
                                        tprGeneral::nameString_combine( "", pvtBp->oakId, "_idle" ) );

        goPtr->set_rootColliEntHeadPtr( &rootGoMeshRef.get_currentFramePos().get_colliEntHead() ); //- 先这么实现...



    //-- 务必在 mesh:"root" 之后 ---
    goPtr->goPos.set_alti( 0.0f );
    goPtr->goPos.init_by_currentMPos( _mpos );
    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( oakTree_pubBinaryValTypes );
}


/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void OakTree::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void OakTree::rebind( GameObj *_goPtr ){
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void OakTree::OnRenderUpdate( GameObj *_goPtr ){
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
            // 目前来看，永远也不会 移动...

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
void OakTree::OnLogicUpdate( GameObj *_goPtr ){
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
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void OakTree::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

    cout << "OakTree::OnActionSwitch()"
        << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _goPtr );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    //GameObjMesh &rootGoMeshRef = goPtr->goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( _type ){
        case ActionSwitchType::Move_Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            //rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->oakId );
                                    
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}


namespace{//-------------- namespace ------------------//



/* ===========================================================
 *                     apply_a_oakId
 * -----------------------------------------------------------
 */
int apply_a_oakId( int _age, float _fieldWeight, bool _isSingleTrunk ){
    size_t  idx;
    int     randV = gameObjs::apply_a_simpleId( _fieldWeight, 83 );

    if( _isSingleTrunk ){ //- 单树干
        switch( _age ){
            case 1: idx = randV % singleTrunk_age1.size();  return singleTrunk_age1.at(idx);
            case 2: idx = randV % singleTrunk_age2.size();  return singleTrunk_age2.at(idx);
            case 3: idx = randV % singleTrunk_age3.size();  return singleTrunk_age3.at(idx);
            case 4: idx = randV % singleTrunk_age4.size();  return singleTrunk_age4.at(idx);
            default:
                assert(0);
                return 0; //- never touch
        }
    }else{ //- 多树枝
        switch( _age ){
            case 1: idx = randV % multiBranch_age1.size();  return multiBranch_age1.at(idx);
            case 2: idx = randV % multiBranch_age2.size();  return multiBranch_age2.at(idx);
            case 3: idx = randV % multiBranch_age3.size();  return multiBranch_age3.at(idx);
            case 4: idx = randV % multiBranch_age4.size();  return multiBranch_age4.at(idx);
            default:
                assert(0);
                return 0; //- never touch
        }
    }
}



}//------------------ namespace: end ------------------//
}//------------- namespace gameObjs: end ----------------

