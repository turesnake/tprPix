/*
 * ======================= Mushroom.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/bushs/Mushroom.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "assemble_go.h"

//-------------------- Script --------------------//

using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------
//namespace Mushroom_inn {//----------- namespace: Mushroom_inn ----------------//
//}//-------------- namespace: Mushroom_inn end ----------------//


struct Mushroom_PvtBinary{
    int        tmp {};
};



void Mushroom::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Mushroom_PvtBinary>();

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );


    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Mushroom::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Mushroom::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Mushroom::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}



void Mushroom::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Mushroom_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}


void Mushroom::bind( GameObj &goRef_ ){}
void Mushroom::rebind( GameObj &goRef_ ){}
void Mushroom::OnLogicUpdate( GameObj &goRef_ ){}



void Mushroom::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

