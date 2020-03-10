/*
 * ========================= Fence.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/artifacts/Fence.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "assemble_go.h"

//-------------------- Script --------------------//

using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------


struct Fence_PvtBinary{
    int   tmp {};
};



void Fence::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Fence_PvtBinary>();


    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );

        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Fence::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Fence::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Fence::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Fence::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Fence::rebind( GameObj &goRef_ ){
}


void Fence::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Fence_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}



void Fence::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Fence_PvtBinary>();
    //=====================================//

    // 什么也没做...
}



void Fence::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        tprAssert(0);
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Fence_PvtBinary>();
    //=====================================//

    //auto dir = goRef_.actionDirection.get_newVal();
    //auto brokenLvl = goRef_.brokenLvl.get_newVal();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.goMeshSet.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.set_animActionEName( AnimActionEName::Idle );
            goMeshRef.bind_animAction();
            break;
        default:
            break;
            //-- 并不报错，什么也不做...

    }

    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...


}


}//------------- namespace gameObjs: end ----------------

