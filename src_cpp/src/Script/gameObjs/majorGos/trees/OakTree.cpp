/*
 * ========================= OakTree.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/trees/OakTree.h"

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

namespace OakTree_inn {//-------------- namespace: OakTree_inn ------------------//

    //--- 将所有 oakId 分类，方便分配 ---
    std::vector<size_t> multiBranch_age1   { 0 };
    std::vector<size_t> singleTrunk_age1   { 1, 2 };
    //---
    std::vector<size_t> multiBranch_age2   { 3, 4 };
    std::vector<size_t> singleTrunk_age2   { 5, 6 };
    //---
    std::vector<size_t> multiBranch_age3   { 7, 8, 9, 10, 11 };
    std::vector<size_t> singleTrunk_age3   { 12, 13, 14, 15, 16 };
    //---
    std::vector<size_t> multiBranch_age4   { 16 }; //- 暂无
    std::vector<size_t> singleTrunk_age4   { 16 }; //- 暂无


    //===== funcs =====//
    size_t apply_a_oakId( int _age, float fieldWeight_, bool _isSingleTrunk );


}//------------------ namespace: OakTree_inn end ------------------//


/* ===========================================================
 *                 init_in_autoMod
 * -----------------------------------------------------------
 */
void OakTree::init_in_autoMod(  goSpecId_t specID_,
                                GameObj &goRef_,
                                const IntVec2 &mpos_,
					            float fieldWeight_,
					            const MapAltitude &alti_,
					            const Density &_density ){

    //================ go.pubBinary ================//
    goRef_.pubBinary.init( oakTree_pubBinaryValTypes );


    //================ go.pvtBinary =================//
    goRef_.resize_pvtBinary( sizeof(OakTree_PvtBinary) );
    OakTree_PvtBinary  *pvtBp = reinterpret_cast<OakTree_PvtBinary*>(goRef_.get_pvtBinaryPtr());

        pvtBp->age = gameObjs::apply_treeAge_by_density( _density );
        pvtBp->isSingleTRunk = gameObjs::apply_isSingleTRunk( fieldWeight_ );
        pvtBp->oakId = OakTree_inn::apply_a_oakId( pvtBp->age, fieldWeight_, pvtBp->isSingleTRunk );
        //...

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
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
                                        gameObjs::apply_isFlipOver( fieldWeight_ ) //- isFlipOver
                                        );

        rootGoMeshRef.bind_animAction( "oakTree", 
                                        tprGeneral::nameString_combine( "", pvtBp->oakId, "_idle" ) );

        goRef_.set_rootColliEntHeadPtr( &rootGoMeshRef.get_currentFramePos().get_colliEntHead() ); //- 先这么实现...


    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &OakTree::OnRenderUpdate, _1 );   
    goRef_.LogicUpdate  = std::bind( &OakTree::OnLogicUpdate,  _1 );
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &OakTree::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Idle );
                //- 当前树木只有一种动画，就是永久待机...

    //================ go self vals =================//

    //--- 小树，中树 可以被其它go 穿过，成年树不行 ---
    if( pvtBp->age <= 2 ){
        goRef_.set_collision_isBePass( true );
    }else{
        goRef_.set_collision_isBePass( false );
    }
    
    //-- 务必在 mesh:"root" 之后 ---
    goRef_.goPos.init_by_currentMPos( mpos_ );
    //...
}


/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void OakTree::bind( GameObj &goRef_ ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void OakTree::rebind( GameObj &goRef_ ){
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void OakTree::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    OakTree_PvtBinary  *pvtBp = OakTree::rebind_ptr( goRef_ );

    //=====================================//
    //           test: AI
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
void OakTree::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    OakTree_PvtBinary  *pvtBp = OakTree::rebind_ptr( goRef_ );
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void OakTree::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    cout << "OakTree::OnActionSwitch()"
        << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    OakTree_PvtBinary  *pvtBp = OakTree::rebind_ptr( goRef_ );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    //GameObjMesh &rootGoMeshRef = goRef_.goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Move_Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            //rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->oakId );
                                    
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}


namespace OakTree_inn {//-------------- namespace: OakTree_inn ------------------//



/* ===========================================================
 *                     apply_a_oakId
 * -----------------------------------------------------------
 */
size_t apply_a_oakId( int _age, float fieldWeight_, bool _isSingleTrunk ){
    size_t  idx;
    size_t     randV = gameObjs::apply_a_simpleId( fieldWeight_, 83 );

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



}//------------------ namespace: OakTree_inn end ------------------//
}//------------- namespace gameObjs: end ----------------

