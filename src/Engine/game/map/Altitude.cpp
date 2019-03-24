/*
 * ====================== Altitude.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Altitude.h"

//-------------------- C --------------------//
#include <cassert>

//------------------- Engine --------------------//
#include "srcs_engine.h"
#include "config.h"


/* ===========================================================
 *                         set
 * -----------------------------------------------------------
 * param: _ppos -- pix ppos
 * param: _freqBig -- 记录在具体 ecoSys 实例中
 * param: _freqSml -- 记录在具体 ecoSys 实例中
 */
void Altitude::set( const IntVec2 &_ppos, const EcoSys *_ecosysPtr ){

    //const float &freqBig = _ecosys.altiPerlin_freqBig;
    //const float &freqSml = _ecosys.altiPerlin_freqSml;

    float freqBig = 2.0;
    float freqSml = 8.0;
    float freqSS = 16.0;
            //-- freq 是动态的，受 ecosys 影响。
            //  但这个值 应该渐渐变化，暂时设为固定值。
    
    float seaLvl = 0.0; //- 海平面，tmp...
                      //- 

    float waterStep = 5.0; //- 水下梯度，tmp...
    float landStep  = 14.0; //- 陆地梯度，tmp...

    int    tmpVal;
    int    tmpLvl;
    //------------------//
    //      val
    //------------------//
    float vx = (float)_ppos.x / (float)PIXES_PER_SECTION;
    float vy = (float)_ppos.y / (float)PIXES_PER_SECTION;

    float pnValBig = esrc::gameSeed.pn_alti.noise(  vx * freqBig, 
                                                    vy * freqBig,
                                                    0.2) * 200 - 100 - seaLvl; // [-80.0, 80.0]

    float pnValSml = esrc::gameSeed.pn_alti.noise(  vx * freqSml, 
                                                    vy * freqSml,
                                                    0.5) * 100 - 50 - seaLvl; // [-80.0, 80.0]

    float pnValSS = esrc::gameSeed.pn_alti.noise(  vx * freqSS, 
                                                    vy * freqSS,
                                                    0.8) * 60 - 30 - seaLvl; // [-80.0, 80.0]

    tmpVal = (int)float( pnValBig + pnValSml + pnValSS ); //- [-200 + seaLvl, 200 + seaLvl]
    if( tmpVal > 100 ){ //- 抹平头尾
        tmpVal = 100;
    }else if( tmpVal < -100 ){
        tmpVal = -100;
    }
    this->val = tmpVal; // [-100, 100]

    //------------------//
    //      lvl
    //------------------//
    if( this->val < 0 ){ //- under water
        tmpLvl = (int)floor((float)tmpVal/waterStep);
        if( tmpLvl < -5 ){ //- 抹平 水域底部
            tmpLvl = -5;
        }
    }else{ //- land
        tmpLvl = (int)floor((float)tmpVal/landStep);
        if( tmpLvl > 2 ){ //- 抹平 陆地高区
            tmpLvl = 2;
        }
    }
    this->lvl = tmpLvl;
}




