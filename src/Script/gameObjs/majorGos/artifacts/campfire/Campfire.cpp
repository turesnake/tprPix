/*
 * ========================= Campfire.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/artifacts/campfire/Campfire.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Lib --------------------//
#include "tprGeneral.h"

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




// 目前主要用来存储 smoke gomesh 数据
class GoMesh_PvtBinary{
public:
    GoMesh_PvtBinary()=default;
    bool isSmoke {false};
    size_t animFrameCount {0}; // 动画计数器
};



struct Campfire_PvtBinary{
    int   tmp {};

    //--- smoke gomesh ---
    animSubspeciesId_t smokeSubId {};
    int smoke_create_count {0}; // 每隔 60帧，生成一个 smoke gomesh
    size_t smokeGomeshIdx {0};

};


namespace campfire_inn {//----------- namespace: campfire_inn ----------------//

    void update_for_smokeGoMesh( GameObj &goRef_, GameObjMesh &smokeGoMesh_, GoMesh_PvtBinary *goMeshBp_ );

}//-------------- namespace: campfire_inn end ----------------//



void Campfire::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Campfire_PvtBinary>();

    // DyParam 中传入的 subid 暂时是无意义的，自行指定
    //animSubspeciesId_t baseSubId = esrc::apply_a_random_animSubspeciesId( "campfire_base", AnimLabel::Default, 7 );
    //animSubspeciesId_t fireSubId = esrc::apply_a_random_animSubspeciesId( "campfire_fire", AnimLabel::Default, 7 );

    pvtBp->smokeSubId = esrc::apply_a_random_animSubspeciesId( "campfire_smoke", AnimLabel::Default, 7 );


    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( goDataPtr->isMultiGoMesh ); // must not single gomesh
    //const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin()); // only one

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );

    if( auto retOpt = goDataPtr->get_brokenLvl(); retOpt.has_value() ){
        goRef_.brokenLvl.reset( retOpt.value() );
    }else{
        tprAssert(0);
    }

                        
    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//


    for( const auto &uptrRef : goDataPtr->goMeshDataUPtrs ){
        const GoDataEntForCreate &goDataEntRef = *uptrRef;

        goRef_.creat_new_goMesh( 
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


    GameObjMesh &rootGoMesh = goRef_.get_goMeshRef("root");
    auto *root_pvtBp = rootGoMesh.init_pvtBinary<GoMesh_PvtBinary>();
    root_pvtBp->isSmoke = false; 

    GameObjMesh &fireGoMesh = goRef_.get_goMeshRef("fire");
    auto *fire_pvtBp = fireGoMesh.init_pvtBinary<GoMesh_PvtBinary>();
    fire_pvtBp->isSmoke = false; 


    /*
    //-- 制作 mesh 实例: "root": campfire_base
    GameObjMesh &rootGoMesh = goRef_.creat_new_goMesh("root", //- gmesh-name
                                baseSubId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
    auto *root_pvtBp = rootGoMesh.init_pvtBinary<GoMesh_PvtBinary>();
    root_pvtBp->isSmoke = false; 


    GameObjMesh &fireGoMesh = goRef_.creat_new_goMesh("fire", //- gmesh-name
                                fireSubId,
                                AnimActionEName::Burn,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.1,  //- zOff: 在 base 上方
                                true //- isVisible
                                );
    auto *fire_pvtBp = fireGoMesh.init_pvtBinary<GoMesh_PvtBinary>();
    fire_pvtBp->isSmoke = false; 
    */

        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Campfire::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Campfire::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Campfire::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::Burn );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Campfire::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Campfire::rebind( GameObj &goRef_ ){
}


void Campfire::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Campfire_PvtBinary>();

    //------------------------//
    // 定期生成一个 smoke gomesh 
    pvtBp->smoke_create_count++;
    if( pvtBp->smoke_create_count > 90 ){
        pvtBp->smoke_create_count = 0;

        std::string goMeshName = tprGeneral::nameString_combine("smoke_", pvtBp->smokeGomeshIdx, "");
        pvtBp->smokeGomeshIdx++;

        GameObjMesh &smokeGoMesh = goRef_.creat_new_goMesh(goMeshName, //- gmesh-name
                                pvtBp->smokeSubId,
                                AnimActionEName::Burn,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                glm::dvec2{}, //- pposoff
                                0.2,  //- zOff: 在 fire 上方
                                true //- isVisible
                                );
        smokeGoMesh.set_alti( 70.0 );
        auto *smoke_pvtBp = smokeGoMesh.init_pvtBinary<GoMesh_PvtBinary>();
        smoke_pvtBp->isSmoke = true;
    }

    //------------------------//
    // 每帧更新所有 smoke-gomesh 数据
    auto &goMeshs = goRef_.get_goMeshs();
    for( auto &[name, goMeshUPtr] : goMeshs ){
        GameObjMesh &goMeshRef = *goMeshUPtr;
        auto *goMeshPvtBp = goMeshRef.get_pvtBinaryPtr<GoMesh_PvtBinary>();
        if( goMeshPvtBp->isSmoke ){

            // 如果此 gomesh，动画播放完毕，将删除它
            auto [playType, playState] = goMeshRef.get_animAction_state();
            tprAssert( playType == AnimAction::PlayType::Once );
            if( playState == AnimAction::State::Stop ){
                goMeshs.erase( name );
                continue; // MUST
            }
            // update
            campfire_inn::update_for_smokeGoMesh( goRef_, goMeshRef, goMeshPvtBp );
        }
    }


    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}

void Campfire::OnLogicUpdate( GameObj &goRef_ ){
}


// 暂未实现
void Campfire::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
        cout << "Campfire::OnActionSwitch" << endl;
        tprAssert(0);
}



namespace campfire_inn {//----------- namespace: campfire_inn ----------------//



void update_for_smokeGoMesh( GameObj &goRef_, GameObjMesh &smokeGoMesh_, GoMesh_PvtBinary *goMeshBp_ ){

    goMeshBp_->animFrameCount++;
    double animDCount = static_cast<double>(goMeshBp_->animFrameCount);

    smokeGoMesh_.accum_alti( 0.6 ); // 不停增高
    smokeGoMesh_.accum_zOff( 0.001 ); // zOff 递增，确保能覆盖 低处的 smoke
    smokeGoMesh_.accum_pposOff( glm::dvec2{ 0.005 * animDCount, 0.0 } ); // 随着不停升高，飘向右侧，表现风
}



}//-------------- namespace: campfire_inn end ----------------//
}//------------- namespace gameObjs: end ----------------

