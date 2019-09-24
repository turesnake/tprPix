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
#include "ParamBinary.h"
#include "MapSurfaceSpec.h"
#include "MapSurfaceRandLvl.h"
#include "RenderPool.h"
#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 
#include "Script/gameObjs/create_go_oth.h"


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


struct MapSurfaceLower_PvtBinary{
    animSubspeciesId_t subspeciesId {};
    //size_t   lichen_ForestId {0};
            //- 简单的从 几种款式中，随机挑选一款 [0,7]
    int tmp {};
    //===== padding =====//
    //...
};

/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void MapSurfaceLower::init_in_autoMod(GameObj &goRef_,
                                const ParamBinary &dyParams_ ){


    //================ dyParams =================//
    auto *msParamPtr = dyParams_.get_binaryPtr<DyParams_MapSurface>();

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<MapSurfaceLower_PvtBinary>();

    pvtBp->subspeciesId = esrc::apply_a_random_animSubspeciesId(MapSurfaceLowSpec_2_str( msParamPtr->spec ),
                                                                std::vector<AnimLabel>{ MapSurfaceRandLvl_2_AnimLabel( msParamPtr->lvl ) },
                                                                msParamPtr->randVal );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        auto &rootGoMeshRef = goRef_.creat_new_goMesh("root", //- gmesh-name
                                    pvtBp->subspeciesId,
                                    "idle",
                                    RenderLayerType::MapSurfaceLower, //- 固定zOff值
                                    &esrc::get_shaderRef(ShaderType::MapSurface),  // pic shader
                                    glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                    0.0,  //- off_z
                                    true ); //- isVisible

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &MapSurfaceLower::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &MapSurfaceLower::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
            //- 当前 mapSurfaceLower 只有一种动画，就是永久待机...

    //================ go self vals =================//
        
    //...    

}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void MapSurfaceLower::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<MapSurfaceLower_PvtBinary>();

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

    //auto &rootGoMeshRef = goRef_.get_goMeshRef("root");
    //rootGoMeshRef


}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void MapSurfaceLower::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "MapSurfaceLower::OnActionSwitch" << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<MapSurfaceLower_PvtBinary>();
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    //GameObjMesh &rootGoMeshRef = goRef_.goMeshs.at("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            //rootGoMeshRef.bind_animFrameSet( "norman" );
            //rootGoMeshRef.getnc_animFrameIdxHandle().bind_idle( pvtBp->lichen_ForestId );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
}


}//------------- namespace gameObjs: end ----------------

