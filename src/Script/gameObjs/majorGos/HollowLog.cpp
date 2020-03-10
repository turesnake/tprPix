/*
 * ========================= HollowLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/HollowLog.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "GoSpecFromJson.h"
#include "assemble_go.h"

//-------------------- Script --------------------//

using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------


struct HollowLog_PvtBinary{
    int   tmp {};
};



void HollowLog::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<HollowLog_PvtBinary>();

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &HollowLog::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &HollowLog::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &HollowLog::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void HollowLog::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void HollowLog::rebind( GameObj &goRef_ ){
}


void HollowLog::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<HollowLog_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}



void HollowLog::OnLogicUpdate( GameObj &goRef_ ){
}



void HollowLog::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
        tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

