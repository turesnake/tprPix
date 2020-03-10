/*
 * ========================= Trough.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/artifacts/trough/Trough.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "assemble_go.h"

//-------------------- Script --------------------//

using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------


struct Trough_PvtBinary{
    int   tmp {};
};



void Trough::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Trough_PvtBinary>();


    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Trough::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Trough::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Trough::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}


void Trough::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Trough_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}



void Trough::bind( GameObj &goRef_ ){}
void Trough::rebind( GameObj &goRef_ ){}
void Trough::OnLogicUpdate( GameObj &goRef_ ){}

void Trough::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
        tprAssert(0);
}




}//------------- namespace gameObjs: end ----------------

