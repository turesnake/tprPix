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
                    const double &ecoObj_densitySeaLvlOff_,
                    const std::vector<double> *ecoObj_densityDivideValsPtr_ ){
    //-----------//
    //   seaLvl
    //-----------//
    double seaLvlOff = ecoObj_densitySeaLvlOff_; //- [-20.0, 20.0]
    double freqSeaLvl = 0.003125 * 0.5; //- 10*10 个 chunk，构成一个 perlin 晶格
    double pnValSeaLvl = simplex_noise2( fieldMPos_.x * freqSeaLvl, 
                                        fieldMPos_.y * freqSeaLvl ) * 15.0 + seaLvlOff; // [-20.0, 20.0]
    //-----------//
    //   pnVal
    //-----------//
    const glm::dvec2 &densitySeed_pposOff = esrc::get_gameSeed().get_densitySeed_pposOff();
    double freq = 0.03125 * 0.5; //- 1*1 个 chunk，构成一个 perlin 晶格
    double x = static_cast<double>(fieldMPos_.x) + densitySeed_pposOff.x;
    double y = static_cast<double>(fieldMPos_.y) + densitySeed_pposOff.y;
    y *= 1.3; //- 在纵向上适当压缩
    double pnVal = simplex_noise2(x*freq, y*freq) * 100.0 - pnValSeaLvl; // [-120.0, 120.0]

    if( pnVal > 100.0 ){
        pnVal = 100.0;
    }
    if( pnVal < -100.0 ){
        pnVal = 100.0;
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


