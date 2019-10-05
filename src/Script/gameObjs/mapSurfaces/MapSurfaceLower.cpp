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
#include "DyParam.h"
#include "MapSurfaceSpec.h"
#include "MapSurfaceRandLvl.h"
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
namespace msl_inn {//------------------ namespace: msl_inn ---------------------//


    struct MapSurfaceLower_PvtBinary{
        //animSubspeciesId_t subspeciesId {};
        //size_t   lichen_ForestId {0};
                //- 简单的从 几种款式中，随机挑选一款 [0,7]
        int tmp {};
        //===== padding =====//
        //...
    };


}//--------------------- namespace: msl_inn end ------------------------//

/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void MapSurfaceLower::init_in_autoMod(GameObj &goRef_,
                                const DyParam &dyParams_ ){


    //================ dyParams =================//
    auto *msParamPtr = dyParams_.get_binaryPtr<DyParams_MapSurface>();

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<msl_inn::MapSurfaceLower_PvtBinary>();

    animSubspeciesId_t subspeciesId {};
    double          entRandVal      {}; // not field randVal
    size_t          meshNameCount  {0};

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    const auto &randMesh = esrc::get_a_mapSurfaceRandMeshData( msParamPtr->lvl, msParamPtr->randVal );
    const auto &randMeshDatas = randMesh.get_data();

    //------ root mesh ------//
    // a special invisible mesh. 
    // case oth meshs will be erase.
    goRef_.creat_new_goMesh( "root", //- gmesh-name
                            esrc::get_emptyPixId(),
                            "idle",
                            RenderLayerType::MapSurfaceLower, //- 固定zOff值
                            &esrc::get_shaderRef(ShaderType::MapSurface),  // pic shader
                            glm::dvec2{0.0, 0.0}, //- pposoff
                            0.0,  //- off_z
                            false ); //- isVisible

    //------ oth meshs ------//
    for( auto it = randMeshDatas.cbegin(); it!= randMeshDatas.cend(); it++ ){
        meshNameCount++;
        
        const auto &mapEntRef = esrc::getnc_memMapEntRef( dpos_2_mpos(goRef_.get_dpos() + it->dposOff) );

            //--- 临时且简陋的检测，未来会被强化 -----
            if( mapEntRef.get_isBorder() ){
                continue;
            }

            //-- 试验，仅在 density 中段 种石块 --
            
            int absDensityLvl = std::abs( mapEntRef.get_density().get_lvl() );
            /*
            if( absDensityLvl > 1 ){
                continue;
            }else if( absDensityLvl == 1 ){
                size_t randV = cast_2_size_t(floor(mapEntRef.get_uWeight() * 13.3 + 1359.67));
                if( (randV % 100) > 70   ){ // 70%
                    continue;
                }
            }
            */
            if( absDensityLvl != 1 ){
                continue;
            }





        subspeciesId = esrc::apply_a_random_animSubspeciesId(   
                                    MapSurfaceLowSpec_2_str( msParamPtr->spec ), // "mapSurfaceLow_whiteRock"
                                    std::vector<AnimLabel>{ mapSurface::mapSurfaceRandMeshLvl_2_AnimLabel( it->meshLvl ) }, // MapEnt_1m1
                                    mapEntRef.get_uWeight() 
                                    );

        goRef_.creat_new_goMesh(tprGeneral::nameString_combine("m", meshNameCount, ""), //- gmesh-name
                                subspeciesId,
                                "idle",
                                RenderLayerType::MapSurfaceLower, //- 固定zOff值
                                &esrc::get_shaderRef(ShaderType::MapSurface),  // pic shader
                                it->dposOff, //- pposoff
                                0.0,  //- off_z
                                true ); //- isVisible
    }

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &MapSurfaceLower::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &MapSurfaceLower::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void MapSurfaceLower::OnRenderUpdate( GameObj &goRef_ ){

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
    tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

