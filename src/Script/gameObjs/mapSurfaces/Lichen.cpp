/*
 * ========================= Lichen.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/mapSurfaces/Lichen.h"

//-------------------- C --------------------//
#include <cassert> //- assert
#include <cmath>

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

    //===== funcs =====//
    int apply_a_lichenId( float _fieldWeight );
    bool apply_isFlipOver( float _fieldWeight );

}//------------------ namespace: end ------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void Lichen::init( GameObj *_goPtr,
                    float _fieldWeight ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;


    //-------- go.pvtBinary ---------//
    goPtr->resize_pvtBinary( sizeof(Lichen_PvtBinary) );
    pvtBp = (Lichen_PvtBinary*)goPtr->get_pvtBinaryPtr(); //- 绑定到本地指针

        pvtBp->lichenId = apply_a_lichenId( _fieldWeight );


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &Lichen::OnRenderUpdate, &lichen, _1 );   
    goPtr->LogicUpdate  = std::bind( &Lichen::OnLogicUpdate,  &lichen, _1 );
    
    //-------- actionSwitch ---------//
    goPtr->actionSwitch.bind_func( std::bind( &Lichen::OnActionSwitch, &lichen, _1, _2 ) );
    goPtr->actionSwitch.signUp( ActionSwitchType::Move_Idle );
            //- 当前 lichen 只有一种动画，就是永久待机...

    //-------- go self vals ---------//
    goPtr->species = Lichen::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::AbsFixed; //- 无法移动
    goPtr->weight = 1.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_1 );   //- lichen一律无法移动
    goPtr->move.set_MoveType( true ); //- tmp

    goPtr->goPos.set_alti( 0.0f );

    goPtr->set_collision_isDoPass( false );
    goPtr->set_collision_isBePass( false );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &rootGoMeshRef = goPtr->creat_new_goMesh( "root", "lichen" );
        rootGoMeshRef.init( goPtr ); 
        rootGoMeshRef.set_pic_zOff( true, ViewingBox::mapSurface_zOff ); //- 设置 固定zOff值
        rootGoMeshRef.picMesh.set_shader_program( &esrc::rect_shader );
        //rootGoMeshRef.shadowMesh.set_shader_program( &esrc::rect_shader ); //- 没有 shadow 时不用设置
        //-- bind animFrameSet / animFrameIdxHandle --
        rootGoMeshRef.animFrameIdxHandle.bind_cycle(pvtBp->lichenId,   //- 起始图元帧序号
                                                pvtBp->lichenId,   //- 结束图元帧序号
                                                pvtBp->lichenId,   //- 入口图元帧序号  
                                                true //- isOrder
                                                );
                        //-- 在未来，要制作一种 静态 动画类型
                        //...

        rootGoMeshRef.isVisible = true;
        rootGoMeshRef.isCollide = true;
        rootGoMeshRef.isFlipOver = apply_isFlipOver( _fieldWeight ); //- tmp

        //-- goMesh pos in go --
        rootGoMeshRef.pposOff = glm::vec2{ 0.0f, 0.0f }; //- 此 goMesh 在 go 中的 坐标偏移 
        rootGoMeshRef.off_z = 0.0f;  //- 作为 0号goMesh,此值必须为0

    //...

    //-------- go.pubBinary ---------//
    goPtr->pubBinary.init( lichen_pubBinaryValTypes );
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Lichen::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Lichen::rebind( GameObj *_goPtr ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Lichen::OnRenderUpdate( GameObj *_goPtr ){
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

        
        goMeshRef.picMesh.refresh_translate();
        goMeshRef.picMesh.refresh_scale_auto(); //- 没必要每帧都执行
        esrc::renderPool_mapSurfaces.push_back( (ChildMesh*)&(goMeshRef.picMesh) );

        // mapSurface gos 一律没有 shadow
    }
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void Lichen::OnLogicUpdate( GameObj *_goPtr ){
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
void Lichen::OnActionSwitch( GameObj *_goPtr, ActionSwitchType _type ){

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
            rootGoMeshRef.animFrameIdxHandle.bind_cycle(pvtBp->lichenId,   //- 起始图元帧序号
                                                        pvtBp->lichenId,   //- 结束图元帧序号
                                                        pvtBp->lichenId,   //- 入口图元帧序号  
                                                        true //- isOrder
                                                        );
            break;


        default:
            break;
            //-- 并不报错，什么也不做...

    }


}


namespace{//-------------- namespace ------------------//



/* ===========================================================
 *                     apply_a_oakId   tmp
 * -----------------------------------------------------------
 * 这组方法很临时。不够好...
 * param: _fieldWeight -- [-100.0, 100.0]
 */
int apply_a_lichenId( float _fieldWeight ){

    int randV = static_cast<int>(floor(_fieldWeight)) * 3 + 977;
    return randV % 8; //- 目前 只有 8个 frame...
                      //  未来需要更规范的写法...
}


bool apply_isFlipOver( float _fieldWeight ){
    int randV = static_cast<int>(floor(_fieldWeight)) * 3 + 911;
    return ((randV%2)==0);
}





}//------------------ namespace: end ------------------//
}//------------- namespace gameObjs: end ----------------

