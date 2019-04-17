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


//---------- static ----------//
int Density::minVal {-3};
int Density::maxVal {3};


/* ===========================================================
 *                         set
 * -----------------------------------------------------------
 */
void Density::set( const IntVec2 &_fieldMPos ){

    //-----------//
    //   seaLvl
    //-----------//
    float seaLvlOff = -10.0; //- 让 高地 更多一些
    float freqSeaLvl = 0.003125 * 0.5; //- 10*10 个 chunk，构成一个 perlin 晶格
    float pnValSeaLvl = simplex_noise2( _fieldMPos.x * freqSeaLvl, 
                                        _fieldMPos.y * freqSeaLvl ) * 15.0 + seaLvlOff; // [-20.0, 20.0]

    //-----------//
    //   
    //-----------//
    float freq = 0.03125 * 0.5; //- 1*1 个 chunk，构成一个 perlin 晶格
    float x = static_cast<float>(_fieldMPos.x) + esrc::gameSeed.densitySeed_pposOff.x;
    float y = static_cast<float>(_fieldMPos.y) + esrc::gameSeed.densitySeed_pposOff.y;
    y *= 1.3; //- 在纵向上适当压缩
    float pnVal = simplex_noise2(x*freq, y*freq) * 100.0 - pnValSeaLvl; // [-120.0, 120.0]


    this->lvl = static_cast<int>(floor(pnVal / 25.0));

    // [-3, 3]
    if( this->lvl > Density::maxVal ){
        this->lvl = Density::maxVal;
    }
    if( this->lvl < Density::minVal ){
        this->lvl = Density::minVal;
    }
}







