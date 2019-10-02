/*
 * ======================= FieldRim.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/oth/FieldRim.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "ParamBinary.h"
#include "RenderPool.h"
#include "create_go_oth.h"

#include "esrc_shader.h" 
#include "esrc_chunk.h"
#include "esrc_animFrameSet.h"
#include "esrc_mapSurfaceRand.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------
namespace fr_inn {//------------------ namespace: fr_inn ---------------------//

    struct FieldRim_PvtBinary{
        int tmp {};
        //===== padding =====//
        //...
    };


}//--------------------- namespace: fr_inn end ------------------------//

/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void FieldRim::init_in_autoMod(GameObj &goRef_,
                                const ParamBinary &dyParams_ ){

    //================ dyParams =================//
    //================ go.pvtBinary =================//
    // ignore...

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    //------ root mesh ------//
    goRef_.creat_new_goMesh( "root", //- gmesh-name
                            esrc::get_fieldRimId(),
                            "idle",
                            RenderLayerType::MapSurfaceLower, //- 固定zOff值
                            &esrc::get_shaderRef(ShaderType::MapSurface),  // pic shader
                            glm::dvec2{0.0, 0.0}, //- pposoff
                            0.5,  //- off_z
                            true ); //- isVisible


    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &FieldRim::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &FieldRim::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void FieldRim::OnRenderUpdate( GameObj &goRef_ ){
    goRef_.render_all_goMesh();
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void FieldRim::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

