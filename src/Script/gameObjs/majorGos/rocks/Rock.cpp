/*
 * ========================== Rock.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/rocks/Rock.h"

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
//namespace Rock_inn {//----------- namespace: Rock_inn ----------------//
//}//-------------- namespace: Rock_inn end ----------------//


struct Rock_PvtBinary{
    animSubspeciesId_t subspeciesId {};
    int        tmp {};
};


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Rock::init(GameObj &goRef_, const DyParam &dyParams_ ){

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
    auto *pvtBp = goRef_.init_pvtBinary<Rock_PvtBinary>();
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
                                it->dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
    }
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Rock::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Rock::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Rock::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Rock::bind( GameObj &goRef_ ){
}

/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Rock::rebind( GameObj &goRef_ ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Rock::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Rock_PvtBinary>();

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
void Rock::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<Rock_PvtBinary>();
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void Rock::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "Rock::OnActionSwitch" << endl;
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Rock_PvtBinary>();
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.bind_animAction( pvtBp->subspeciesId, goRef_.get_actionDirection(), "idle" );
            break;

        default:
            break;
            tprAssert(0);
    }

    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...


}


}//------------- namespace gameObjs: end ----------------

