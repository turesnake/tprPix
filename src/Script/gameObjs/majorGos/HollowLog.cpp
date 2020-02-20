/*
 * ========================= HollowLog.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/HollowLog.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "GoSpecFromJson.h"

#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------


struct HollowLog_PvtBinary{
    int   tmp {};
};



void HollowLog::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<HollowLog_PvtBinary>();

    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;
    //tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
    //const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin()); // only one

     //-- set lAltiRange ---
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    goRef_.set_pos_lAltiRange( goSpecFromJsonRef.get_lAltiRange( goDataPtr->goAltiRangeLabel ) );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );

    auto retOpt1 = goDataPtr->get_brokenLvl();
    tprAssert( retOpt1.has_value() );

    if( auto retOpt2 = goDataPtr->get_brokenLvl(); retOpt2.has_value() ){
        goRef_.brokenLvl.reset( retOpt2.value() );
    }else{
        tprAssert(0);
    }
                        

    //----- gomeshs -----//
    for( const auto &uptrRef : goDataPtr->goMeshDataUPtrs ){
        const GoDataEntForCreate &goDataEntRef = *uptrRef;
        auto &goMeshRef = goRef_.creat_new_goMesh( 
                                goDataEntRef.goMeshName,
                                goDataEntRef.subspeciesId,
                                goDataEntRef.animActionEName,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                goDataEntRef.zOff,  //- zOff
                                true //- isVisible
                                );
    }

    /*
    //-- 制作唯一的 mesh 实例: "root" --
    goRef_.creat_new_goMesh("root", //- gmesh-name
                                goDataEntRef.subspeciesId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
    */
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &HollowLog::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &HollowLog::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &HollowLog::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void HollowLog::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void HollowLog::rebind( GameObj &goRef_ ){
}


void HollowLog::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<HollowLog_PvtBinary>();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}



void HollowLog::OnLogicUpdate( GameObj &goRef_ ){
}



void HollowLog::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
        cout << "HollowLog::OnActionSwitch" << endl;
        tprAssert(0);
}


}//------------- namespace gameObjs: end ----------------

