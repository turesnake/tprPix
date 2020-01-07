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

//-------------------- tpr --------------------//
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


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------
namespace grass_inn {//----------- namespace: grass_inn ----------------//


    class GoMesh_PvtBinary{
    public:
        bool    isWindClockWorking {false}; 
        size_t  windDelayStepCount      {}; // dynamicVal, count down
    };


    class Grass_PvtBinary{
    public:
        inline void insert_goMeshData(  const std::string name_, animSubspeciesId_t id_ )noexcept{
            auto outPair = this->goMeshSubSpeciesIds.insert({ name_, id_ });
            tprAssert( outPair.second );
        }
        inline animSubspeciesId_t get_goMesh_animSubspeciesId( const std::string goMeshName_ )const noexcept{
            tprAssert( this->goMeshSubSpeciesIds.find(goMeshName_) != this->goMeshSubSpeciesIds.end() );
            return this->goMeshSubSpeciesIds.at( goMeshName_ );
        }
        //----- vals -----//
        size_t     windClockCheckStepCount {}; // count down
        size_t     oldWindClockCount {0};
    private:
        std::unordered_map<std::string, animSubspeciesId_t> goMeshSubSpeciesIds {};
    };


    //--- randUVal pools ---//
    std::vector<size_t> goMesh_windDelaySteps_pool {}; // 100 ents
    size_t              goMesh_windDelaySteps_poolIdx {};

    std::vector<size_t> go_windDelaySteps_pool {}; // 100 ents
    size_t              go_windDelaySteps_poolIdx {};

    std::vector<std::string>    windAnimActionName_pool {};
    size_t                      windAnimActionName_poolIdx {};

    //----- flags -----//
    bool                isFstCalled {true};
    //===== funcs =====//
    void    init()noexcept;
    size_t  apply_a_goMesh_windDelayStep()noexcept;
    size_t  apply_a_go_windDelayStep()noexcept;
    const std::string &apply_a_windAnimActionName()noexcept;

}//-------------- namespace: grass_inn end ----------------//



void Grass::init(GameObj &goRef_, const DyParam &dyParams_ ){

    std::string funcName = "Grass::init()";

    //-- simple way ...
    if( grass_inn::isFstCalled ){
        grass_inn::isFstCalled = false;
        grass_inn::init();
    }


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<grass_inn::Grass_PvtBinary>();

    //================ dyParams =================//    
    tprAssert( dyParams_.get_typeHash() == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate * goDataPtr = bpParamPtr->goDataPtr;
    tprAssert( goDataPtr->isMultiGoMesh ); // must multi gomesh


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( goDataPtr->direction );
    goRef_.set_brokenLvl( goDataPtr->brokenLvl );


    //----- windClock -----//
    pvtBp->windClockCheckStepCount = grass_inn::apply_a_go_windDelayStep();

    //----- gomeshs -----//
    std::string         goMeshName {};
    size_t              meshNameCount {0};
    animSubspeciesId_t     subspeciesId {};

    auto &windClockRef = esrc::get_windClock();
    
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

        pvtBp->insert_goMeshData( goMeshName, subspeciesId );
        //---
        auto &goMeshRef = goRef_.creat_new_goMesh(goMeshName,
                                subspeciesId,
                                "idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                //it->dposOff, //- pposoff
                                goDataEntRef.dposOff,
                                0.0,  //- zOff
                                true //- isVisible
                                );


        auto *goMeshPvtBPtr = goMeshRef.get_pvtBinary().init<grass_inn::GoMesh_PvtBinary>();
        goMeshPvtBPtr->windDelayStepCount = grass_inn::apply_a_goMesh_windDelayStep();

        goMeshRef.bind_reset_playSpeedScale( std::bind( &WindClock::get_playSpeedScale,
                                                        &windClockRef,
                                                        goDataEntRef.windDelayIdx ) );

    }
        
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
    auto *pvtBp = goRef_.get_pvtBinaryPtr<grass_inn::Grass_PvtBinary>();

    auto dir = goRef_.get_actionDirection();
    auto brokenLvl = goRef_.get_brokenLvl();

    //=====================================//
    //            windClock         
    //-------------------------------------//
    auto &windClockRef = esrc::get_windClock();
    bool isNeedStartWind {false};

    pvtBp->windClockCheckStepCount--;
    if( pvtBp->windClockCheckStepCount <= 0 ){ // nodeframe
        pvtBp->windClockCheckStepCount = grass_inn::apply_a_go_windDelayStep(); // new val
        //- check windClock count
        size_t newWindClockCount = windClockRef.get_clockCount();
        if( newWindClockCount != pvtBp->oldWindClockCount ){
            pvtBp->oldWindClockCount = newWindClockCount;
            //---
            isNeedStartWind = true;
        }
    }


    for( auto &pair : goRef_.get_goMeshs() ){// {string, uptr<GameObjMesh>}
        auto &goMeshRef = *(pair.second);
        auto *goMeshPvtBPtr = goMeshRef.get_pvtBinary().get<grass_inn::GoMesh_PvtBinary>();
        //---
        if( isNeedStartWind ){
            if( goMeshRef.get_animAction_state().second == AnimAction::State::Working ){
                continue; // skip
            }
            if( !goMeshPvtBPtr->isWindClockWorking ){
                goMeshPvtBPtr->isWindClockWorking = true;
            }
        }
        //---
        if( goMeshPvtBPtr->isWindClockWorking ){
            goMeshPvtBPtr->windDelayStepCount--;
            if( goMeshPvtBPtr->windDelayStepCount <= 0 ){ // nodeFrame
                goMeshPvtBPtr->windDelayStepCount = grass_inn::apply_a_goMesh_windDelayStep(); // new val
                goMeshPvtBPtr->isWindClockWorking = false;
                //---
                goMeshRef.bind_animAction(  pvtBp->get_goMesh_animSubspeciesId( pair.first ),
                                            dir, brokenLvl, 
                                            grass_inn::apply_a_windAnimActionName() );
            }
        }
    }

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
    std::uniform_int_distribution<size_t> uDistribution_goMesh( 1, 9 ); // can't begin with 0
    //std::uniform_int_distribution<size_t> uDistribution_go(     10, 40 );
    std::uniform_int_distribution<size_t> uDistribution_go(     1, 5 );
    
    //=== goMesh windDelaySteps pool ===//
    goMesh_windDelaySteps_pool.reserve( 100 );
    for( int i=0; i<100; i++ ){
        goMesh_windDelaySteps_pool.push_back( uDistribution_goMesh(randEngine) );
    }
    goMesh_windDelaySteps_poolIdx = 0;

    //=== go windDelaySteps pool ===//
    go_windDelaySteps_pool.reserve( 100 );
    for( int i=0; i<100; i++ ){
        go_windDelaySteps_pool.push_back( uDistribution_go(randEngine) );
    }
    go_windDelaySteps_poolIdx = 0;


    //=== windAnimActionName pool ===//
    windAnimActionName_pool.reserve(50);
    windAnimActionName_pool.insert( windAnimActionName_pool.end(), 17, "wind_1" );
    //windAnimActionName_pool.insert( windAnimActionName_pool.end(), 1, "wind_2" );
    windAnimActionName_pool.insert( windAnimActionName_pool.end(), 1, "idle" );
    std::shuffle( windAnimActionName_pool.begin(), windAnimActionName_pool.end(), randEngine );
    windAnimActionName_poolIdx = 0;
}


size_t apply_a_goMesh_windDelayStep()noexcept{
    goMesh_windDelaySteps_poolIdx++;
    if( goMesh_windDelaySteps_poolIdx >= goMesh_windDelaySteps_pool.size() ){
        goMesh_windDelaySteps_poolIdx = 0;
    }
    return goMesh_windDelaySteps_pool.at(goMesh_windDelaySteps_poolIdx);
}


size_t apply_a_go_windDelayStep()noexcept{
    go_windDelaySteps_poolIdx++;
    if( go_windDelaySteps_poolIdx >= go_windDelaySteps_pool.size() ){
        go_windDelaySteps_poolIdx = 0;
    }
    return go_windDelaySteps_pool.at(go_windDelaySteps_poolIdx);
}


const std::string &apply_a_windAnimActionName()noexcept{
    windAnimActionName_poolIdx++;
    if( windAnimActionName_poolIdx >= windAnimActionName_pool.size() ){
        windAnimActionName_poolIdx = 0;
    }
    return windAnimActionName_pool.at( windAnimActionName_poolIdx );
}


}//-------------- namespace: grass_inn end ----------------//
}//------------- namespace gameObjs: end ----------------

