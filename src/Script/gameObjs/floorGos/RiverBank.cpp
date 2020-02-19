/*
 * ======================= RiverBank.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/floorGos/RiverBank.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <functional>
#include <string>
#include <vector>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "RenderPool.h"
#include "create_go_oth.h"
#include "dyParams.h"

#include "esrc_shader.h" 
#include "esrc_chunk.h"
#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------
namespace riverBank_inn {//------------------ namespace: riverBank_inn ---------------------//

    double calc_goMeshZOff( size_t mapEntUWeight_ );


}//--------------------- namespace: riverBank_inn end ------------------------//


struct FloorGo_PvtBinary{

    int tmp {};
    //===== padding =====//
    //...
};


void RiverBank::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<FloorGo_PvtBinary>();


    


    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_RiverBank).hash_code() );
    const auto *bpParamPtr = dyParams_.get_binaryPtr<DyParams_RiverBank>();



    
    // 最简状态，内部分配，未来要依靠 DyParams 提供的参数 
    animSubspeciesId_t rootSubId = esrc::apply_a_random_animSubspeciesId( "riverBank", AnimLabel::Default, bpParamPtr->mapEntUWeight );




    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( NineDirection::Center ); // 手动设置

    

    // 依靠 uWeight，计算 zOff，从而确保，相交的 floorgo 之间，恒定的覆盖关系
    double goMeshZOff = riverBank_inn::calc_goMeshZOff( bpParamPtr->mapEntUWeight ); // (0.0, 0.1)


    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                rootSubId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                glm::dvec2{0.0, 0.0}, //- pposoff
                                //goMeshZOff,  //- zOff
                                -100.0,
                                true //- isVisible
                                );

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &RiverBank::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &RiverBank::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}



void RiverBank::OnRenderUpdate( GameObj &goRef_ ){

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void RiverBank::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


namespace riverBank_inn {//------------------ namespace: riverBank_inn ---------------------//



double calc_goMeshZOff( size_t mapEntUWeight_ ){

    // 获得一个 小数值 (0.0, 0.1)
    double rd = static_cast<double>(mapEntUWeight_) / 71.17;
    double integer {}; // 不会被使用
    double fract = modf(rd, &integer) / 10.0; // (0.0, 0.1)
    return ( floorGoLayer_2_goMesh_baseZOff(FloorGoLayer::L_4) + fract );
}




}//--------------------- namespace: riverBank_inn end ------------------------//
}//------------- namespace gameObjs: end ----------------

