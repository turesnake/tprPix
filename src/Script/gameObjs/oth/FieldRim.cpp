/*
 * ======================= FieldRim.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/oth/FieldRim.h"

//-------------------- Engine --------------------//
#include "DyParam.h"
#include "animSubspeciesId.h"
#include "assemble_go.h"

//-------------------- Script --------------------//


using namespace std::placeholders;



namespace gameObjs{//------------- namespace gameObjs ----------------
namespace fr_inn {//------------------ namespace: fr_inn ---------------------//

    


}//--------------------- namespace: fr_inn end ------------------------//


struct FieldRim_PvtBinary{
    int tmp {};
};



void FieldRim::init(GameObj &goRef_,const DyParam &dyParams_ ){

    //================ dyParams =================//
    //================ go.pvtBinary =================//
    // ignore...

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &FieldRim::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &FieldRim::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}



void FieldRim::OnRenderUpdate( GameObj &goRef_ ){
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}


/* -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void FieldRim::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

