/*
 * ========================= OakTree.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/OakTree.h"

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


    std::uniform_int_distribution<int> uDistribution_bool(0,1);
    std::uniform_int_distribution<int> uDistribution_oakId(0,100);

    //--- 将所有 oakId 分类，方便分配 ---
    std::vector<int> multiBranch_age1   { 0 };
    std::vector<int> singleTrunk_age1   { 1, 2 };
    //---
    std::vector<int> multiBranch_age2   { 3, 4 };
    std::vector<int> singleTrunk_age2   { 5, 6 };
    //---
    std::vector<int> multiBranch_age3   { 7, 8, 9, 10, 11 };
    std::vector<int> singleTrunk_age3   { 12, 13, 14, 15 };
    //---
    std::vector<int> multiBranch_age4   { 17 };
    std::vector<int> singleTrunk_age4   { 16 };


    //===== funcs =====//
    int apply_a_oakId( int _age, bool _isSingleTrunk );


}//------------------ namespace: end ------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void OakTree::init( GameObj *_goPtr, 
                    int _age,
                    bool _isSingleTrunk ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- go.pvtBinary ---------//
    goPtr->resize_pvtBinary( sizeof(OakTree_PvtBinary) );
    pvtBp = (OakTree_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针

        pvtBp->age = _age;
        pvtBp->isSingleTRunk = _isSingleTrunk;
        pvtBp->oakId = apply_a_oakId( _age, _isSingleTrunk );
        //...
        

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &OakTree::OnRenderUpdate, &oakTree, _1 );   
    goPtr->LogicUpdate  = std::bind( &OakTree::OnLogicUpdate,  &oakTree, _1 );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &OakTree::OnActionSwitch, &oakTree, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
                //- 当前树木只有一种动画，就是永久待机...

    //-------- go self vals ---------//
    goPtr->species = OakTree::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::AbsFixed;
    goPtr->weight = 50.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_1 );   //- 树木一律无法移动
    goPtr->move.set_MoveType( true ); //- tmp

    goPtr->goPos.set_alti( 0.0f );

    goPtr->set_collision_isDoPass( false );
    goPtr->set_collision_isBePass( false );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = goPtr->creat_new_goMesh( "root" );
        rootGoMeshRef.init( goPtr ); 
        rootGoMeshRef.picMesh.set_shader_program( &esrc::rect_shader );
        rootGoMeshRef.shadowMesh.set_shader_program( &esrc::rect_shader );
        //-- bind animFrameSet / animFrameIdxHandle --
        rootGoMeshRef.bind_animFrameSet( "oakTree" );
        rootGoMeshRef.animFrameIdxHandle.bind_cycle(pvtBp->oakId,   //- 起始图元帧序号
                                                    pvtBp->oakId,   //- 结束图元帧序号
                                                    pvtBp->oakId,   //- 入口图元帧序号  
                                                    true //- isOrder
                                                    );
                        //-- 在未来，要制作一种 静态 动画类型
                        //...

        rootGoMeshRef.isVisible = true;
        rootGoMeshRef.isCollide = true;
        rootGoMeshRef.isFlipOver = false;

        //-- goMesh pos in go --
        rootGoMeshRef.pposOff = glm::vec2{ 0.0f, 0.0f }; //- 此 goMesh 在 go 中的 坐标偏移 
        rootGoMeshRef.off_z = 0.0f;  //- 作为 0号goMesh,此值必须为0

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
    GameObjMesh &rootGoMeshRef = goPtr->goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( _type ){
        case ActionSwitchType::Move_Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            rootGoMeshRef.animFrameIdxHandle.bind_cycle(pvtBp->oakId,   //- 起始图元帧序号
                                                        pvtBp->oakId,   //- 结束图元帧序号
                                                        pvtBp->oakId,   //- 入口图元帧序号  
                                                        true //- isOrder
                                                        );
                                    //-- 先这么写着。
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}



/* ===========================================================
 *                   create_a_OakTree  
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_OakTree( const IntVec2 &_mpos, int _age, bool _isSingleTrunk ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针
    gameObjs::oakTree.init( goPtr, _age, _isSingleTrunk );
    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );

        esrc::goids_active.insert( goid ); //- tmp

    //- 小树 可以被穿越 --
    if( _age == 1 ){
        goPtr->set_collision_isBePass(true);
    }

    return goid;
}




namespace{//-------------- namespace ------------------//



/* ===========================================================
 *                     apply_a_oakId
 * -----------------------------------------------------------
 */
int apply_a_oakId( int _age, bool _isSingleTrunk ){
    size_t  idx;
    int     randV = uDistribution_oakId(esrc::gameSeed.randEngine);
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

