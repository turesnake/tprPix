/*
 * ====================== GroundGo.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/oth/GroundGo.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "DyParam.h"
#include "RenderPool.h"
#include "create_go_oth.h"
#include "fieldFractType.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "assemble_go.h"

#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"
#include "esrc_VAOVBO.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


void GroundGo::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    // no need

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );
    
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &GroundGo::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &GroundGo::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
}


void GroundGo::OnRenderUpdate( GameObj &goRef_ ){
    goRef_.goMeshSet.render_all_groundGoMesh();
}


void GroundGo::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){ tprAssert(0); }


}//------------- namespace gameObjs: end ----------------

