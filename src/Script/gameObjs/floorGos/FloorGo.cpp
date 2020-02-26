/*
 * ========================= FloorGo.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/floorGos/FloorGo.h"

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
#include "GoSpecFromJson.h"

#include "esrc_shader.h" 
#include "esrc_chunk.h"
#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//




using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs{//------------- namespace gameObjs ----------------
namespace floorGo_inn {//------------------ namespace: floorGo_inn ---------------------//

    double calc_goMeshZOff( size_t mapEntUWeight_, FloorGoLayer layer_ );


}//--------------------- namespace: floorGo_inn end ------------------------//


struct FloorGo_PvtBinary{

    int tmp {};
    //===== padding =====//
    //...
};


void FloorGo::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<FloorGo_PvtBinary>();

    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;


    //-- set lAltiRange ---
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    goRef_.set_pos_lAltiRange( goSpecFromJsonRef.get_lAltiRange( goDataPtr->goAltiRangeLabel ) );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );

    goRef_.set_colliDataFromJsonPtr( goDataPtr->colliDataFromJsonPtr );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
    double goMeshZOff {};
    for( const auto &uptrRef : goDataPtr->goMeshEntUPtrs ){
        const GoDataForCreate::GoMesh &gmRef = *uptrRef;

        // 依靠 uWeight，计算 zOff，从而确保，相交的 floorgo 之间，恒定的覆盖关系
        auto &floorGoLayerOpt = gmRef.get_floorGoLayer();
        tprAssert( floorGoLayerOpt.has_value() );

        goMeshZOff = floorGo_inn::calc_goMeshZOff( bpParamPtr->mapEntUWeight, floorGoLayerOpt.value() ); // (0.0, 0.1)
        
        auto &goMeshRef = goRef_.creat_new_goMesh( 
                                gmRef.get_goMeshName(),
                                gmRef.get_subspeciesId(),
                                gmRef.get_animActionEName(),
                                gmRef.get_renderLayerType(),
                                gmRef.get_shaderType(),  // pic shader
                                gmRef.get_dposOff(), //- pposoff

                                goMeshZOff,  //- zOff

                                gmRef.get_isVisible() //- isVisible
                                );
    } 


    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &FloorGo::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &FloorGo::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}



void FloorGo::OnRenderUpdate( GameObj &goRef_ ){

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* -- 此处用到的 animFrameIdxHdle实例，是每次用到时，临时 生产／改写 的
 * -- 会被 动作状态机 取代...
 */
void FloorGo::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}


namespace floorGo_inn {//------------------ namespace: floorGo_inn ---------------------//



double calc_goMeshZOff( size_t mapEntUWeight_, FloorGoLayer layer_ ){

    // 获得一个 小数值 (0.0, 0.1)
    double rd = static_cast<double>(mapEntUWeight_) / 71.17;
    double integer {}; // 不会被使用
    double fract = modf(rd, &integer) / 10.0; // (0.0, 0.1)
    return ( floorGoLayer_2_goMesh_baseZOff(layer_) + fract );
}




}//--------------------- namespace: floorGo_inn end ------------------------//
}//------------- namespace gameObjs: end ----------------

