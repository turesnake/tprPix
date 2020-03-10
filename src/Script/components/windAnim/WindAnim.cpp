/*
 * ====================== WindAnim.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/components/windAnim/WindAnim.h"

//-------------------- Engine --------------------//
#include "random.h"
#include "esrc_time.h"



namespace component{//------------- namespace component ----------------


//======= static =======//
size_t WindAnim::goMesh_windDelaySteps_poolIdx {0};
size_t WindAnim::go_windDelaySteps_poolIdx {0};

std::vector<size_t> WindAnim::goMesh_windDelaySteps_pool {};
std::vector<size_t> WindAnim::go_windDelaySteps_pool {};


void WindAnim::init_for_static()noexcept{

    std::default_random_engine  randEngine;
    randEngine.seed( get_new_seed() );

    {//=== goMesh windDelaySteps pool ===//
        // can't begin with 0
        std::uniform_int_distribution<size_t> uid_1( 1, 6 );    // 600-ents
        std::uniform_int_distribution<size_t> uid_2( 7, 10 );   // 300-ents
        std::uniform_int_distribution<size_t> uid_3( 11, 13 );  // 100-ents

        WindAnim::goMesh_windDelaySteps_pool.reserve( 1000 );
        for( int i=0; i<600; i++ ){
            WindAnim::goMesh_windDelaySteps_pool.push_back( uid_1(randEngine) );
        }
        for( int i=0; i<300; i++ ){
            WindAnim::goMesh_windDelaySteps_pool.push_back( uid_2(randEngine) );
        }
        for( int i=0; i<100; i++ ){
            WindAnim::goMesh_windDelaySteps_pool.push_back( uid_3(randEngine) );
        }
        std::shuffle(   WindAnim::goMesh_windDelaySteps_pool.begin(),
                        WindAnim::goMesh_windDelaySteps_pool.end(),
                        randEngine ); // 无需 gameSeed.shuffleEngine 
    }

    {//=== go windDelaySteps pool ===//
        // can't begin with 0
        std::uniform_int_distribution<size_t> uid( 1, 6 );

        WindAnim::go_windDelaySteps_pool.reserve( 100 );
        for( int i=0; i<100; i++ ){
            WindAnim::go_windDelaySteps_pool.push_back( uid(randEngine) );
        }
        std::shuffle(   WindAnim::go_windDelaySteps_pool.begin(),
                        WindAnim::go_windDelaySteps_pool.end(),
                        randEngine ); // 无需 gameSeed.shuffleEngine 
    }
}



void WindAnim::init( const std::vector<AnimActionEName> *ActionEName_poolPtr_ )noexcept{

    //----------//
    this->ActionEName_poolPtr = ActionEName_poolPtr_;
    this->windClockCheckStepCount = WindAnim::apply_a_go_windDelayStep(); // new val

    auto &windClockRef = esrc::get_windClock();

    tprAssert( !this->goMeshs.empty() );
    
    for( auto &iUPtr : this->goMeshs ){

        tprAssert( iUPtr->goMeshPtr );
        GameObjMesh &goMeshRef = *iUPtr->goMeshPtr;

        iUPtr->windDelayStepCount = WindAnim::apply_a_goMesh_windDelayStep();

        goMeshRef.bind_reset_playSpeedScale( std::bind( &WindClock::get_playSpeedScale,
                                                        &windClockRef,
                                                        iUPtr->windDelayIdx ) );
    }

}






// call in each render frame
void WindAnim::update()noexcept{

    auto &windClockRef = esrc::get_windClock();
    bool isNeedStartWind {false};


    this->windClockCheckStepCount--;
    if( this->windClockCheckStepCount <= 0 ){ // nodeframe


        this->windClockCheckStepCount = WindAnim::apply_a_go_windDelayStep(); // new val
        //- check windClock count
        size_t newWindClockCount = windClockRef.get_clockCount();

        if( newWindClockCount != this->oldWindClockCount ){
            this->oldWindClockCount = newWindClockCount;
            //---
            isNeedStartWind = true;
        }
    }

    
    for( auto &iUPtr : this->goMeshs ){
        GameObjMesh &goMeshRef = *iUPtr->goMeshPtr;
        
        if( isNeedStartWind ){
            if( goMeshRef.get_animAction_state().second == AnimAction::PlayState::Working ){ // 未来此处将被修改 
                continue; // skip
            }
            if( !iUPtr->isWindClockWorking ){
                iUPtr->isWindClockWorking = true;
            }
        }
        //---
        if( iUPtr->isWindClockWorking ){
            iUPtr->windDelayStepCount--;
            if( iUPtr->windDelayStepCount <= 0 ){ // nodeFrame
                iUPtr->windDelayStepCount = WindAnim::apply_a_goMesh_windDelayStep(); // new val
                iUPtr->isWindClockWorking = false;
                //---
                goMeshRef.set_animActionEName( this->apply_a_windAnimActionEName() );
                goMeshRef.bind_animAction();
                                // 也许不应该在此处调用
                                // 而是在 具象go类，后续的某个 统一位置 调用  
                                // ...
            }
        }
    }

}








}//------------- namespace component: end ----------------

