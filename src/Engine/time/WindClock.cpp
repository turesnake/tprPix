/*
 * ========================= WindClock.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "WindClock.h"


//------------------- CPP --------------------//
#include <random>

//------------------- Engine --------------------// 
#include "random.h"
#include "tprMath.h"
#include "simplexNoise.h"

#include "tprDebug.h"


void WindClock::init()noexcept{

    //std::default_random_engine  randEngine;
    //randEngine.seed( get_new_seed() );
    //std::uniform_int_distribution<size_t> uDistribution( 20, 60 );
    //std::uniform_int_distribution<size_t> uDistribution( 10, 120 );

    //--- clock step pool ---//
    /*
    this->clockStepPool.reserve( 100 );
    for( int i=0; i<100; i++ ){
        this->clockStepPool.push_back( uDistribution(randEngine) );
    }
    //---
    this->clockStepPoolIdx = 0;
    this->reflesh_clockStep();
    */
    {
        double x {0.0};
        double y {};
        double sin1 {};
        double perlin1 {};

        double amplitude_1 { 0.6 };
        double amplitude_2 { 1.0 };
        //---
        double totalLen = TPR_PI * 20.0;
        double frameStep = 0.7;
        size_t frameNums = cast_2_size_t(totalLen / frameStep);
        this->playSpeedScalePool.reserve( frameNums );
        for( size_t i=0; i<frameNums; i++ ){
            x += frameStep;

            sin1 = sin(x) * amplitude_1; 
            perlin1 = simplex_noise2( x*0.3, 107.3 ) * amplitude_2;

            y = (sin1 + perlin1) / (amplitude_1 + amplitude_2) ; // [-1.0, 1.0]
            y = y * 60 + 65; // [20, 70]

            this->clockStepPool.push_back( static_cast<size_t>( floor(y) ) );
        }
        //---
        this->clockStepPoolIdx = 0;
        this->reflesh_clockStep();
    }


    //--- play speed scale pool ---//
    {
        double x {0.0};
        double y {};
        double sin1 {};
        double perlin1 {};
        double perlin2 {};

        double amplitude_1 { 0.6 };
        double amplitude_2 { 1.0 };
        double amplitude_3 { 0.5 };
        //---
        double totalLen = TPR_PI * 20.0;
        double frameStep = 0.04;
        size_t frameNums = cast_2_size_t(totalLen / frameStep);
        this->playSpeedScalePool.reserve( frameNums );
        for( size_t i=0; i<frameNums; i++ ){
            x += frameStep;
            
            sin1    = sin(x) * amplitude_1;         
            perlin1 = simplex_noise2( x*0.2,    15.537 ) * amplitude_2;
            perlin2 = simplex_noise2( x + 51.3, 7.1 ) * amplitude_3;
            //--
            y = (sin1 + perlin1 + perlin2) / (amplitude_1 + amplitude_2 + amplitude_3); // [-1.0, 1.0]
            y = y * 0.5 + 0.7; // [0.2, 1.2]
            
            this->playSpeedScalePool.push_back( y );
        }
        //---
        this->playSpeedScalePoolIdx = 0;
    }
}




