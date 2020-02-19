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
#include "groundGoEntType.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"

#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"
#include "esrc_VAOVBO.h"

//-------------------- Script --------------------//

using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------
namespace groundGo_inn {//------------------ namespace: groundGo_inn ---------------------//

    

    //===== funcs =====//
    AnimLabel Job_GroundGoEntType_2_AnimLabel( GroundGoEntType type_ )noexcept;

}//--------------------- namespace: groundGo_inn end ------------------------//


struct GroundGo_PvtBinary{
    int tmp {};
};


void GroundGo::init(GameObj &goRef_, const DyParam &dyParams_ ){


    //================ dyParams =================//
    auto *msParamPtr = dyParams_.get_binaryPtr<DyParams_GroundGo>();

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<GroundGo_PvtBinary>();


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( NineDirection::Center );

    animSubspeciesId_t subspeciesId {};
    std::string     meshName {};
    size_t          nameIdx  {0};

    const Job_GroundGoEnt *jgEntPtr {nullptr};
    const auto &container = msParamPtr->job_fieldPtr->get_job_groundGoEnts();


    //------ meshs ------//
    for( auto it = container.cbegin(); it != container.cend(); it++ ){
        if( it == container.cbegin() ){
            meshName = "root";
        }else{
            nameIdx++;
            meshName = tprGeneral::nameString_combine("ent", nameIdx, "");
        }

        jgEntPtr = it->get();
        subspeciesId = esrc::apply_a_random_animSubspeciesId(   
                    "groundGo",
                    groundGo_inn::Job_GroundGoEntType_2_AnimLabel( jgEntPtr->groundType ),
                    msParamPtr->fieldUWeight  + jgEntPtr->uWeight
                    );

        auto &goMeshRef = goRef_.creat_new_goMesh( meshName, //- gmesh-name
                                subspeciesId,
                                AnimActionEName::Idle,
                                RenderLayerType::GroundGo, //- 固定zOff值
                                &esrc::get_shaderRef(ShaderType::GroundColor),  // pic shader
                                jgEntPtr->dposOff,
                                0.0,  //- zOff
                                true //- isVisible
                                ); 
        goMeshRef.set_colorTableId( jgEntPtr->colorTableId );
    }
    

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &GroundGo::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &GroundGo::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );

    //================ go self vals =================//
    //... 
}


void GroundGo::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_groundGoMesh();
}


void GroundGo::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


namespace groundGo_inn {//------------------ namespace: groundGo_inn ---------------------//


AnimLabel Job_GroundGoEntType_2_AnimLabel( GroundGoEntType type_ )noexcept{
    switch (type_){
        case GroundGoEntType::MapEnt:       return AnimLabel::Sml;
        case GroundGoEntType::HalfField:    return AnimLabel::Mid;
        case GroundGoEntType::Field:        return AnimLabel::Big;
        case GroundGoEntType::SimpleField:  return AnimLabel::Big; //- tmp
        default:
            tprAssert(0);
            return AnimLabel::Sml;
    }
}



}//--------------------- namespace: groundGo_inn end ------------------------//
}//------------- namespace gameObjs: end ----------------

