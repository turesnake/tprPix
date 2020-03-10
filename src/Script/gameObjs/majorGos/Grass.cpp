/*
 * ========================== Grass.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/Grass.h"

//-------------------- CPP --------------------//
#include <random>

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "random.h"
#include "simplexNoise.h"
#include "dyParams.h"

//-------------------- Script --------------------//
#include "Script/components/windAnim/WindAnim.h"

using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------
namespace grass_inn {//----------- namespace: grass_inn ----------------//

    std::vector<AnimActionEName> windAnimActionEName_pool {};
    
    //----- flags -----//
    bool    isFstCalled {true};
    //===== funcs =====//
    void    init()noexcept;

}//-------------- namespace: grass_inn end ----------------//


class Grass_PvtBinary{
public:
    Grass_PvtBinary():
        windAnimUPtr( std::make_unique<component::WindAnim>() )
        {}

    // Exist but forbidden to call
    Grass_PvtBinary( const Grass_PvtBinary & ){ tprAssert(0); }
    Grass_PvtBinary &operator=( const Grass_PvtBinary & );

    //------- vals -------//
    int tmp {0};
    std::unique_ptr<component::WindAnim> windAnimUPtr {};
};

Grass_PvtBinary &Grass_PvtBinary::operator=( const Grass_PvtBinary & ){
    tprAssert(0);
    return *this; // never reach
}



void Grass::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //-- simple way ...
    if( grass_inn::isFstCalled ){
        grass_inn::isFstCalled = false;
        grass_inn::init();
    }

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Grass_PvtBinary>();

    //================ dyParams =================//    
    tprAssert( dyParams_.get_typeHash() == typeid(DyParams_GoDataForCreate).hash_code() );
    const DyParams_GoDataForCreate *bpParamPtr = dyParams_.get_binaryPtr<DyParams_GoDataForCreate>();
    const GoDataForCreate * goDataPtr = bpParamPtr->goDataPtr;


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->get_direction() );
    goRef_.brokenLvl.reset( goDataPtr->get_brokenLvl() );

    goRef_.set_colliDataFromJsonPtr( goDataPtr->get_colliDataFromJsonPtr() );

    //----- gomeshs -----//
    for( const auto &sptrRef : goDataPtr->get_goMeshs_autoInit() ){
        const GoDataForCreate::GoMeshBase &gmRef = *sptrRef;

        auto &goMeshRef = goRef_.goMeshSet.creat_new_goMesh( 
                                gmRef.get_goMeshName(),
                                gmRef.get_subspeciesId(),
                                gmRef.get_animActionEName(),
                                gmRef.get_renderLayerType(),
                                gmRef.get_shaderType(),  // pic shader
                                gmRef.get_dposOff(), //- pposoff
                                gmRef.get_zOff(),  //- zOff
                                1151, // uweight tmp
                                gmRef.get_isVisible() //- isVisible
                                );


        // 先收集 所有参与 风吹动画的 gomesh 数据
        pvtBp->windAnimUPtr->insert_goMesh( &goMeshRef, gmRef.get_windDelayIdx() );
    }

    //----- component: windAnim -----//
    // 正式 init
    pvtBp->windAnimUPtr->init( &grass_inn::windAnimActionEName_pool );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Grass::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Grass::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    // not support switch anim from outside yet
    goRef_.actionSwitch.bind_func( std::bind( &Grass::OnActionSwitch,  _1, _2 ) );
    //goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
}



void Grass::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Grass_PvtBinary>();


    //=====================================//
    //            windClock         
    //-------------------------------------//
    // goMeshRef.bind_animAction() 已经在 update 内被自动调用了
    // 这不是一个好实现 
    // 在未来，可能要被转移到 其他地方
    // ...
    pvtBp->windAnimUPtr->update();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}



void Grass::bind( GameObj &goRef_ ){ /* do nothing... */ }
void Grass::rebind( GameObj &goRef_ ){ /* do nothing... */ }
void Grass::OnLogicUpdate( GameObj &goRef_ ){ /* do nothing... */ }
void Grass::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){ /* do nothing... */ }



namespace grass_inn {//----------- namespace: grass_inn ----------------//



void init()noexcept{
    std::default_random_engine  randEngine;
    randEngine.seed( get_new_seed() );

    //=== windAnimActionName pool ===//
    windAnimActionEName_pool.reserve(20);
    windAnimActionEName_pool.insert( windAnimActionEName_pool.end(), 17, AnimActionEName::Wind );
    windAnimActionEName_pool.insert( windAnimActionEName_pool.end(), 1, AnimActionEName::Idle );
    std::shuffle( windAnimActionEName_pool.begin(), windAnimActionEName_pool.end(), randEngine );
}




}//-------------- namespace: grass_inn end ----------------//
}//------------- namespace gameObjs: end ----------------

