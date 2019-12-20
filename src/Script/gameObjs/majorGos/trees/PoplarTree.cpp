/*
 * ========================= PoplarTree.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/trees/PoplarTree.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "Density.h"
#include "animSubspecId.h"
#include "dyParams.h"

#include "tprAssert.h"
#include "esrc_shader.h" 
#include "esrc_gameSeed.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------


//namespace PoplarTree_inn {//----------- namespace: PoplarTree_inn ----------------//
    //inline std::uniform_int_distribution<int>     uDistribution_int(      3,   50 );
    //inline std::uniform_real_distribution<double> uDistribution_double( -1.3, 1.3 );
//}//-------------- namespace: PoplarTree_inn end ----------------//


struct PoplarTree_PvtBinary{
    animSubspecId_t subspecId {};
    int        tmp {};
};


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void PoplarTree::init(GameObj &goRef_, const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<PoplarTree_PvtBinary>();



    //================ dyParams =================//

    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;

    tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
    const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin());
    pvtBp->subspecId = goDataEntRef.subspecId;
                             
    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                pvtBp->subspecId,
                                "idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &PoplarTree::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &PoplarTree::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &PoplarTree::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void PoplarTree::bind( GameObj &goRef_ ){
}

/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void PoplarTree::rebind( GameObj &goRef_ ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void PoplarTree::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<PoplarTree_PvtBinary>();

    //=====================================//
    //            AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* ===========================================================
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void PoplarTree::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<PoplarTree_PvtBinary>();
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void PoplarTree::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "PoplarTree::OnActionSwitch" << endl;
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<PoplarTree_PvtBinary>();
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.bind_animAction( pvtBp->subspecId, goRef_.get_actionDirection(), "idle" );
            break;

        default:
            break;
            tprAssert(0);
    }

    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...


}


}//------------- namespace gameObjs: end ----------------

