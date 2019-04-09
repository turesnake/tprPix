/*
 * ====================== Density.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Density.h"

//------------------- C --------------------//
#include <cmath>

//------------------- Engine --------------------//
#include "simplexNoise.h"
#include "srcs_engine.h"


/* ===========================================================
 *                         set
 * -----------------------------------------------------------
 */
void Density::set( const IntVec2 &_fieldMPos ){

    //-----------//
    //   seaLvl
    //-----------//
    float freqSeaLvl = 0.003125 * 0.5; //- 10*10 个 chunk，构成一个 perlin 晶格
    float pnValSeaLvl = simplex_noise2( _fieldMPos.x * freqSeaLvl, 
                                        _fieldMPos.y * freqSeaLvl ) * 10.0; // [-20.0, 20.0]

    //pnValSeaLvl = 0.0;

    //-----------//
    //   
    //-----------//
    float freq = 0.03125 * 0.5; //- 1*1 个 chunk，构成一个 perlin 晶格
    float x = static_cast<float>(_fieldMPos.x) + esrc::gameSeed.densitySeed_pposOff.x;
    float y = static_cast<float>(_fieldMPos.y) + esrc::gameSeed.densitySeed_pposOff.y;
    float pnVal = simplex_noise2(x*freq, y*freq) * 100.0 - pnValSeaLvl; // [-120.0, 120.0]


    this->lvl = static_cast<int>(floor(pnVal / 25.0));

    // [-3, 3]
    if( this->lvl > 3 ){
        this->lvl = 3;
    }
    if( this->lvl < -3 ){
        this->lvl = -3;
    }
}







