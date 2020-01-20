/*
 * ========================== Grass.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/Grass.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>
#include <random>

//-------------------- Lib --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "Density.h"
#include "animSubspeciesId.h"
#include "tprAssert.h"
#include "tprCast.h"
#include "create_go_oth.h"
#include "random.h"
#include "simplexNoise.h"
#include "dyParams.h"

#include "esrc_time.h"
#include "esrc_shader.h" 
#include "esrc_chunk.h"
#include "esrc_gameSeed.h"
#include "esrc_animFrameSet.h"
#include "esrc_colorTableSet.h"


//-------------------- Script --------------------//
#include "Script/components/windAnim/WindAnim.h"



using namespace std::placeholders;

#include "tprDebug.h" 


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
    tprAssert( dyParams_.get_typeHash() == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate * goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( goDataPtr->isMultiGoMesh ); // must multi gomesh


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );
    goRef_.brokenLvl.reset( goDataPtr->brokenLvl );


    //----- gomeshs -----//
    std::string         goMeshName {};
    size_t              meshNameCount {0};
    
    size_t idx {0};
    for( auto it = goDataPtr->goMeshDataUPtrs.cbegin(); it != goDataPtr->goMeshDataUPtrs.cend(); it++ ){

        const GoDataEntForCreate &goDataEntRef = *(*it);

        //--- goMesh name ---//
        if( it == goDataPtr->goMeshDataUPtrs.cbegin() ){
            goMeshName = "root";
        }else{
            goMeshName = tprGeneral::nameString_combine("m_", meshNameCount, "");
            meshNameCount++;
        }

        //---
        auto &goMeshRef = goRef_.creat_new_goMesh(goMeshName,
                                goDataEntRef.subspeciesId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                //it->dposOff, //- pposoff
                                goDataEntRef.dposOff,
                                0.0,  //- zOff
                                true //- isVisible
                                );


        // 先收集 所有参与 风吹动画的 gomesh 数据
        pvtBp->windAnimUPtr->insert_goMesh( &goMeshRef, goDataEntRef.windDelayIdx );
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
    goRef_.render_all_goMesh();
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

