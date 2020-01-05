/*
 * ========================= FloorGo.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/floorGos/FloorGo.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
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
namespace floorGo_inn {//------------------ namespace: floorGo_inn ---------------------//


    struct FloorGo_PvtBinary{
        //animSubspeciesId_t subspeciesId {};
        //size_t   lichen_ForestId {0};
                //- 简单的从 几种款式中，随机挑选一款 [0,7]

        animSubspeciesId_t subspeciesId {};
        int tmp {};
        //===== padding =====//
        //...
    };


}//--------------------- namespace: floorGo_inn end ------------------------//


void FloorGo::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<floorGo_inn::FloorGo_PvtBinary>();

    //================ dyParams =================//

    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
    const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin());
    pvtBp->subspeciesId = goDataEntRef.subspeciesId;

    

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Center );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                pvtBp->subspeciesId,
                                "idle",
                                RenderLayerType::Floor, //- 固定zOff值
                                &esrc::get_shaderRef(ShaderType::Floor),  // pic shader
                                //glm::dvec2{ 0.0, 0.0 }, //- pposoff
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );

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


}//------------- namespace gameObjs: end ----------------

