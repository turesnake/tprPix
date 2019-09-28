/*
 * ====================== GroundGo.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/oth/GroundGo.h"

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
#include "Job_GroundGoEnt.h"

#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 



using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------


struct GroundGo_PvtBinary{
    //animSubspeciesId_t subspeciesId {};
    //size_t   lichen_ForestId {0};
            //- 简单的从 几种款式中，随机挑选一款 [0,7]
    int tmp {};
    //===== padding =====//
    //...
};



AnimLabel Job_GroundGoEntType_2_AnimLabel( Job_GroundGoEntType type_ ){
    switch (type_){
        case Job_GroundGoEntType::Mapent:       return AnimLabel::Sml;
        case Job_GroundGoEntType::HalfField:    return AnimLabel::Mid;
        case Job_GroundGoEntType::Field:        return AnimLabel::Big;
        case Job_GroundGoEntType::SimpleField:  return AnimLabel::Big; //- tmp
        default:
            tprAssert(0);
            return AnimLabel::Sml;
    }
}




/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void GroundGo::init_in_autoMod(GameObj &goRef_,
                                const ParamBinary &dyParams_ ){


    //================ dyParams =================//
    auto *msParamPtr = dyParams_.get_binaryPtr<DyParams_GroundGo>();

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<GroundGo_PvtBinary>();


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    animSubspeciesId_t subspeciesId {};
    size_t nameIdx  {0};

    const Job_GroundGoEnt *jgEntPtr {nullptr};
    const auto &container = msParamPtr->job_fieldPtr->get_job_groundGoEnts();


    //------ "root" mesj --------//
    auto it = container.cbegin();

    jgEntPtr = it->get();
    subspeciesId = esrc::apply_a_random_animSubspeciesId(   
                "groundGo",
                std::vector<AnimLabel>{ Job_GroundGoEntType_2_AnimLabel( jgEntPtr->groundType ) },
                msParamPtr->fieldUWeight + jgEntPtr->uWeight
                );

    auto &rootMeshRef = goRef_.creat_new_goMesh("root", //- gmesh-name
                            subspeciesId,
                            "idle",
                            RenderLayerType::GroundGo, //- 固定zOff值
                            &esrc::get_shaderRef(ShaderType::GroundColor),  // pic shader
                            jgEntPtr->fposOff,
                            0.0,  //- off_z
                            true ); //- isVisible
    rootMeshRef.set_colorTableId( jgEntPtr->colorTableId );

    //------ oth mesh ------//
    for( ; it != container.cend(); it++ ){
        nameIdx++;

        jgEntPtr = it->get();
        subspeciesId = esrc::apply_a_random_animSubspeciesId(   
                    "groundGo",
                    std::vector<AnimLabel>{ Job_GroundGoEntType_2_AnimLabel( jgEntPtr->groundType ) },
                    msParamPtr->fieldUWeight  + jgEntPtr->uWeight
                    );

        auto &goMeshRef = goRef_.creat_new_goMesh(tprGeneral::nameString_combine("ent", nameIdx, ""), //- gmesh-name
                                subspeciesId,
                                "idle",
                                RenderLayerType::GroundGo, //- 固定zOff值
                                &esrc::get_shaderRef(ShaderType::GroundColor),  // pic shader
                                jgEntPtr->fposOff,
                                0.0,  //- off_z
                                true ); //- isVisible
        goMeshRef.set_colorTableId( jgEntPtr->colorTableId );
    }

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &GroundGo::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &GroundGo::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
            //- 当前 mapSurfaceLower 只有一种动画，就是永久待机...

    //================ go self vals =================//
    //... 
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void GroundGo::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_groundGoMesh();
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void GroundGo::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

