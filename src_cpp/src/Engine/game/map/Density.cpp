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
#include "tprAssert.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoObj.h"

//#include "tprDebug.h"

//---------- static ----------//
int Density::minLvl {-3};
int Density::maxLvl {3};


/* ===========================================================
 *                         set
 * -----------------------------------------------------------
 * 根据 ecoObj 预设的方案 来配置 density.lvl
 */
void Density::set( const IntVec2 &fieldMPos_, 
                    const float &ecoObj_densitySeaLvlOff_,
                    const std::vector<float> *ecoObj_densityDivideValsPtr_ ){
    //-----------//
    //   seaLvl
    //-----------//
    float seaLvlOff = ecoObj_densitySeaLvlOff_; //- [-20.0, 20.0]
    float freqSeaLvl = 0.003125f * 0.5f; //- 10*10 个 chunk，构成一个 perlin 晶格
    float pnValSeaLvl = simplex_noise2( fieldMPos_.x * freqSeaLvl, 
                                        fieldMPos_.y * freqSeaLvl ) * 15.0f + seaLvlOff; // [-20.0, 20.0]
    //-----------//
    //   pnVal
    //-----------//
    const glm::vec2 &densitySeed_pposOff = esrc::get_gameSeed().get_densitySeed_pposOff();
    float freq = 0.03125f * 0.5f; //- 1*1 个 chunk，构成一个 perlin 晶格
    float x = static_cast<float>(fieldMPos_.x) + densitySeed_pposOff.x;
    float y = static_cast<float>(fieldMPos_.y) + densitySeed_pposOff.y;
    y *= 1.3f; //- 在纵向上适当压缩
    float pnVal = simplex_noise2(x*freq, y*freq) * 100.0f - pnValSeaLvl; // [-120.0, 120.0]

    if( pnVal > 100.0f ){
        pnVal = 100.0f;
    }
    if( pnVal < -100.0f ){
        pnVal = 100.0f;
    } // [-100.0, 100.0]

    //-----------//
    //   lvl
    //-----------//
    int tmpLvl = Density::minLvl;
    bool  is_find {false};
    for( auto f=ecoObj_densityDivideValsPtr_->cbegin(); 
        f!=ecoObj_densityDivideValsPtr_->cend(); f++ ){
        if( pnVal < *f ){
            is_find = true;
            this->lvl = tmpLvl;
            break;
        }
        tmpLvl++;
    }   
    if( !is_find ){
        this->lvl = Density::maxLvl;
    }
    tprAssert( (this->lvl>=Density::minLvl) && (this->lvl<=Density::maxLvl) );
}


