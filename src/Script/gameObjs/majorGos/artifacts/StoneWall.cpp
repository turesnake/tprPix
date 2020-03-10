/*
 * ========================= StoneWall.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/artifacts/StoneWall.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "GoSpecFromJson.h"
#include "assemble_go.h"

//-------------------- Script --------------------//

using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------


struct StoneWall_PvtBinary{
    int   tmp {};
};



void StoneWall::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<StoneWall_PvtBinary>();


    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );
    
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &StoneWall::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &StoneWall::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &StoneWall::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );

    //================ go self vals =================//   
}



void StoneWall::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<StoneWall_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}


void StoneWall::bind( GameObj &goRef_ ){}
void StoneWall::rebind( GameObj &goRef_ ){}
void StoneWall::OnLogicUpdate( GameObj &goRef_ ){}


void StoneWall::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
        tprAssert(0);
        
}


}//------------- namespace gameObjs: end ----------------

