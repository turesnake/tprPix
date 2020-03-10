/*
 * ========================= FloorGo.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/floorGos/FloorGo.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "GoSpecFromJson.h"
#include "assemble_go.h"

//-------------------- Script --------------------//


using namespace std::placeholders;


namespace gameObjs{//------------- namespace gameObjs ----------------

//struct FloorGo_PvtBinary{
//};


void FloorGo::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    //auto *pvtBp = goRef_.init_pvtBinary<FloorGo_PvtBinary>();

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &FloorGo::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &FloorGo::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}


void FloorGo::OnRenderUpdate( GameObj &goRef_ ){
    goRef_.goMeshSet.render_all_floorGoMesh();
}


/* -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void FloorGo::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}

}//------------- namespace gameObjs: end ----------------

