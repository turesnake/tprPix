/*
 * ========================= MapSurfaceLower.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/mapSurfaces/MapSurfaceLower.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 
#include "Script/gameObjs/create_go_oth.h"


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void MapSurfaceLower::init_in_autoMod(GameObj &goRef_,
                                const ParamBinary &dyParams_ ){

    //================ go.pvtBinary =================//
    goRef_.resize_pvtBinary( sizeof(MapSurfaceLower_PvtBinary) );
    auto *pvtBp = reinterpret_cast<MapSurfaceLower_PvtBinary*>(goRef_.get_pvtBinaryPtr());

        //pvtBp->lichen_ForestId = gameObjs::apply_a_simpleId( fieldWeight_, 32 );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        /*
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                "lichen_Forest", 
                                tprGeneral::nameString_combine("", pvtBp->lichen_ForestId, "_idle"),
                                RenderLayerType::MapSurfaces,
                                &esrc::get_rect_shader(),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- off_z
                                true //- isVisible
                                );
        */

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &MapSurfaceLower::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &MapSurfaceLower::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Idle );
            //- 当前 mapSurfaceLower 只有一种动画，就是永久待机...

    //================ go self vals =================//
        
    //-- 务必在 mesh:"root" 之后 ---   
    goRef_.init_goPos_currentDPos( );
    //...    

    //--- MUST ---
    goRef_.init_check();
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void MapSurfaceLower::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = MapSurfaceLower::rebind_ptr( goRef_ );

    //=====================================//
    //              AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    //goRef_.move.RenderUpdate();
            // 目前来看，永远也不会 移动...

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void MapSurfaceLower::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = MapSurfaceLower::rebind_ptr( goRef_ );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    //GameObjMesh &rootGoMeshRef = goRef_.goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Move_Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            //rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->lichen_ForestId );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
}


}//------------- namespace gameObjs: end ----------------

