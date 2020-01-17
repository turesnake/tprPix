/*
 * ======================= Mushroom.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/bushs/Mushroom.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"

#include "tprAssert.h"
#include "esrc_shader.h" 
#include "esrc_gameSeed.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//

using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------
//namespace Mushroom_inn {//----------- namespace: Mushroom_inn ----------------//
//}//-------------- namespace: Mushroom_inn end ----------------//


struct Mushroom_PvtBinary{
    int        tmp {};
};



void Mushroom::init(GameObj &goRef_, const DyParam &dyParams_ ){


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( NineDirection::Center );

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Mushroom_PvtBinary>();


    //================ dyParams =================//
    tprAssert( dyParams_.get_typeHash() == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate * goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( goDataPtr->isMultiGoMesh ); // must multi gomesh


    //----- gomeshs -----//
    std::string         goMeshName {};
    size_t              meshNameCount {0};
    animSubspeciesId_t     subspeciesId {};
    
    size_t idx {0};
    for( auto it = goDataPtr->goMeshDataUPtrs.cbegin(); 
        it != goDataPtr->goMeshDataUPtrs.cend(); it++ ){

        const GoDataEntForCreate &goDataEntRef = *(*it);

        //--- goMesh name ---//
        if( it == goDataPtr->goMeshDataUPtrs.cbegin() ){
            goMeshName = "root";
        }else{
            goMeshName = tprGeneral::nameString_combine("m_", meshNameCount, "");
            meshNameCount++;
        }

        subspeciesId = goDataEntRef.subspeciesId;

        //---
        auto &goMeshRef = goRef_.creat_new_goMesh(goMeshName,
                                subspeciesId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                //it->dposOff, //- pposoff
                                goDataEntRef.dposOff,
                                0.0,  //- zOff
                                true //- isVisible
                                );

    }
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Mushroom::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Mushroom::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Mushroom::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Mushroom::bind( GameObj &goRef_ ){
}

/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Mushroom::rebind( GameObj &goRef_ ){
}


void Mushroom::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Mushroom_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}



void Mushroom::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Mushroom_PvtBinary>();
    //=====================================//

    // 什么也没做...
}



void Mushroom::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    tprAssert(0);

}


}//------------- namespace gameObjs: end ----------------

