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

#include "tprAssert.h"
#include "esrc_shader.h" 
#include "esrc_gameSeed.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc_all.h" 

using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------
//namespace Mushroom_inn {//----------- namespace: Mushroom_inn ----------------//
//}//-------------- namespace: Mushroom_inn end ----------------//


struct Mushroom_PvtBinary{
    //animSubspeciesId_t subspeciesId {}; // 在未来，它会是个 vector
    int        tmp {};
};


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Mushroom::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ dyParams =================//
    const DyParams_Field *msParamPtr {nullptr};
    //---
    size_t typeHash = dyParams_.get_typeHash();
    if( typeHash == typeid(DyParams_Field).hash_code() ){
        msParamPtr = dyParams_.get_binaryPtr<DyParams_Field>();

    }else{
        tprAssert(0); //- 尚未实现
    }


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Mushroom_PvtBinary>();
   
    const auto &job_goMeshs = *(msParamPtr->job_goMeshsPtr);


    std::string         goMeshName {};
    size_t              meshNameCount {0};
    animSubspeciesId_t  subspeciesId {};

    for( auto it=job_goMeshs.cbegin(); it!=job_goMeshs.cend(); it++ ){// each job_goMesh

        //--- goMesh name ---//
        if( it == job_goMeshs.cbegin() ){
            goMeshName = "root";
        }else{
            goMeshName = tprGeneral::nameString_combine("m_", meshNameCount, "");
            meshNameCount++;
        }

        subspeciesId = it->subspecId;

        goRef_.creat_new_goMesh(goMeshName,
                                subspeciesId,
                                "idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                //glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                it->fposOff,
                                0.0,  //- off_z
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

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Mushroom::bind( GameObj &goRef_ ){
}

/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Mushroom::rebind( GameObj &goRef_ ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Mushroom::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Mushroom_PvtBinary>();

    //=====================================//
    //            AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    //goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void Mushroom::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Mushroom_PvtBinary>();
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void Mushroom::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    tprAssert(0);

}


}//------------- namespace gameObjs: end ----------------

