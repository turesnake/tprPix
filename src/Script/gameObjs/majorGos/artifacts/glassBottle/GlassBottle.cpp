/*
 * ======================= GlassBottle.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/artifacts/glassBottle/GlassBottle.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"

#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------


struct GlassBottle_PvtBinary{
    int   tmp {};
};



void GlassBottle::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<GlassBottle_PvtBinary>();


    animSubspeciesId_t bottleSubId = esrc::apply_a_random_animSubspeciesId( "glassBottle_bottle", AnimLabel::Default, 1 );
    animSubspeciesId_t waterSubId = esrc::apply_a_random_animSubspeciesId( "glassBottle_water", AnimLabel::Default, 1 );
    animSubspeciesId_t innSpaceSubId = esrc::apply_a_random_animSubspeciesId( "glassBottle_innSpace", AnimLabel::Default, 1 );

    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
    const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin()); // only one

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );

    if( auto retOpt = goDataPtr->get_brokenLvl(); retOpt.has_value() ){
        goRef_.brokenLvl.reset( retOpt.value() );
    }else{
        tprAssert(0);
    }

                        

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
    //-- goMesh: "root": bottle
    goRef_.creat_new_goMesh("root", //- gmesh-name
                                //goDataEntRef.subspeciesId,
                                bottleSubId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );

    //-- goMesh: "water"
    goRef_.creat_new_goMesh("water", //- gmesh-name
                                //goDataEntRef.subspeciesId,
                                waterSubId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                -1.0,  //- zOff 在 bottle 后方
                                true //- isVisible
                                );

    //-- goMesh: "innSpace"
    goRef_.creat_new_goMesh("innSpace", //- gmesh-name
                                //goDataEntRef.subspeciesId,
                                innSpaceSubId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                -2.0,  //- zOff 在 water 后方
                                true //- isVisible
                                );

        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &GlassBottle::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &GlassBottle::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &GlassBottle::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void GlassBottle::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void GlassBottle::rebind( GameObj &goRef_ ){
}


void GlassBottle::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<StoneWall_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}



void GlassBottle::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<StoneWall_PvtBinary>();
    //=====================================//

    // 什么也没做...
}



void GlassBottle::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
        cout << "GlassBottle::OnActionSwitch" << endl;
        tprAssert(0);
}




}//------------- namespace gameObjs: end ----------------

