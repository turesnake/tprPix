/*
 * ========================= SingleFoot.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.27
 *                                        修改 -- 2019.01.27
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/SingleFoot.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 

//-------------------- Script --------------------//
#include "Script/actionHandle/Cycle.h"
#include "Script/resource/srcs_script.h" 

using namespace std::placeholders;

#include "debug.h" 

namespace gameObjs{//------------- namespace gameObjs ----------------

namespace{//-------------- namespace ------------------//


}//------------------ namespace: end ------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- 应该在 调用 init 之前，创建 基础go实例（并添加到 全局容器中）
 * -- 尽量把 具象go类 做得 “工厂化” 一些。
 */
void SingleFoot::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &SingleFoot::RenderUpdate, &single_foot, _1 );   
    goPtr->LogicUpdate  = std::bind( &SingleFoot::LogicUpdate,  &single_foot, _1 );
    goPtr->BeAffect     = std::bind( &SingleFoot::BeAffect,     &single_foot, _1 ); 

    //-------- go self vals ---------//
    goPtr->species = SingleFoot::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->parentId = NULLID;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 2.0f;

    goPtr->isTopGo = true;
    goPtr->isActive = true;
    goPtr->isDirty = false;
    goPtr->isControlByPlayer = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_6 );
    goPtr->move.set_MoveType( true ); //- tmp

    //-------- action／actionHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        GameObjMesh &goMeshRef = goPtr->creat_new_goMesh( "root" );
        goMeshRef.init( goPtr ); 
        goMeshRef.picMesh.set_shader_program( &esrc::rect_shader );
        goMeshRef.shadowMesh.set_shader_program( &esrc::rect_shader );
        goMeshRef.isVisible = true;
        goMeshRef.isCollide = true;
        //-- bind action / actionHandle --
        goMeshRef.bind_action( "singleFootWalk" );
        actionHdle::cycle_obj.bind( goMeshRef.get_actionHandlePtr(), 
                                    1,                //- 起始图元帧序号
                                    7,                //- 结束图元帧序号
                                    1,               //- 入口图元帧序号
                                    std::vector<int>{ 8 }, //- steps
                                    true            //- isStepEqual
                                    ); 

        //-- goMesh pos in go --
        goMeshRef.pposOff = glm::vec2{ 0.0f, 0.0f }; //- 此 goMesh 在 go 中的 坐标偏移 
        goMeshRef.off_z = 0.0f;  //- 作为 0号goMesh,此值必须为0

        goMeshRef.isVisible = true;
        goMeshRef.isCollide = true;
        goMeshRef.isFlipOver = false;
    
    //-------- go.binary ---------//
    goPtr->resize_binary( sizeof(SingleFoot_Binary) );
    bp = (SingleFoot_Binary*)goPtr->get_binaryHeadPtr(); //- 绑定到本地指针

    bp->HP = 100;
    //..
}


/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void SingleFoot::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void SingleFoot::rebind( GameObj *_goPtr ){
}


/* ===========================================================
 *                      RenderUpdate
 * -----------------------------------------------------------
 */
void SingleFoot::RenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == SingleFoot::specId );
    //-- rebind ptr -----
    goPtr = _goPtr;
    bp = (SingleFoot_Binary*)goPtr->get_binaryHeadPtr();

    //=====================================//
    //           test: AI
    //-------------------------------------//
    //...
    /*
    if( goPtr->is_control_by_player == false  ){

        if( step_x_count > 2 ){
            step_x_count = 0;
        }

        if( step_x_count == 0 ){

            flyIns.rad += 0.5f;
            //goPtr->move.set_newflyIns( flyIns );
        }
        step_x_count++;
    }
    */
    
    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goPtr->move.RenderUpdate();


    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    auto ipair = goPtr->goMeshs.begin();
    for( ; ipair!=goPtr->goMeshs.end(); ipair++ ){
        GameObjMesh &goMeshRef = ipair->second;

        //-- 也许不该放在 这个位置 --
        if( goMeshRef.isVisible == false ){
            continue;
        }

        //=== 传参到 scriptBuf : [无参数] ===
        goMeshRef.get_actionHandle_func("update")( goMeshRef.get_actionHandlePtr(), 0);
        //=== 从 scriptBuf 取返回值 : [无返回值] ===

        goMeshRef.shadowMesh.refresh_translate();
        goMeshRef.shadowMesh.refresh_scale_auto(); //- 没必要每帧都执行
        goMeshRef.picMesh.refresh_translate();
        goMeshRef.picMesh.refresh_scale_auto(); //- 没必要每帧都执行

        esrc::renderPool_goMeshs_pic.insert({ goMeshRef.shadowMesh.get_render_z(), (ChildMesh*)&(goMeshRef.shadowMesh) });
        esrc::renderPool_goMeshs_shadow.push_back( (ChildMesh*)&(goMeshRef.picMesh) );
    }
}


/* ===========================================================
 *                        LogicUpdate
 * -----------------------------------------------------------
 */
void SingleFoot::LogicUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == SingleFoot::specId );
    //-- rebind ptr -----
    goPtr = _goPtr;
    bp = (SingleFoot_Binary*)goPtr->get_binaryHeadPtr();
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *                       BeAffect
 * -----------------------------------------------------------
 */
void SingleFoot::BeAffect( GameObj *_goPtr ){
}











}//------------- namespace gameObjs: end ----------------

