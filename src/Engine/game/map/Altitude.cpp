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

    float freqBig = 2.5;
    float freqSml = 8.0;
    float freqSS = 16.0;
            //-- freq 是动态的，受 ecosys 影响。
            //  但这个值 应该渐渐变化，暂时设为固定值。
    
    float seaLvl = -5.0; //- 海平面，tmp...
                      //- 值越小，land 区域越大
                      //- 值越大，water 区域越大

    float waterStep = 5.0; //- 水下梯度，tmp...
    float landStep  = 14.0; //- 陆地梯度，tmp...

    int    tmpVal;
    int    tmpLvl;

    int    tmpSandVal;

    float pnValBig;
    float pnValSml;
    float pnValSS;

    float pnValSandBig;
    float pnValSandSml;
    //------------------//
    //      val
    //------------------//
    float vx = (float)_ppos.x / (float)PIXES_PER_SECTION;
    float vy = (float)_ppos.y / (float)PIXES_PER_SECTION;

    pnValBig = esrc::gameSeed.pn_alti.noise( vx * freqBig, 
                                            vy * freqBig,
                                            0.2) * 200 - 100 - seaLvl; // [-100.0, 100.0]
    
    pnValSml = esrc::gameSeed.pn_alti.noise(vx * freqSml, 
                                            vy * freqSml,
                                            0.5) * 100 - 50 - seaLvl; // [-50.0, 50.0]
    
    pnValSS = esrc::gameSeed.pn_alti.noise( vx * freqSS, 
                                            vy * freqSS,
                                            0.8) * 60 - 30 - seaLvl; // [-30.0, 30.0]

    /*
    pnValSandBig = esrc::gameSeed.pn_alti.noise(vx * freqSml, 
                                                vy * freqSml,
                                                100.0) * 100 - 50 - seaLvl; // [-50.0, 50.0]

    pnValSandSml = esrc::gameSeed.pn_alti.noise(vx * freqSS, 
                                                vy * freqSS,
                                                450.0) * 100 - 50 - seaLvl; // [-50.0, 50.0]
    */


    tmpVal = (int)( pnValBig + pnValSml + pnValSS ); //- [-200 + seaLvl, 200 + seaLvl]

    

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

    //------------------//
    //     isSand
    //------------------//
    this->isSand = false; //- MUST
    
    /*
    //tmpSandVal = (int)(pnValSSS); //- 专门用来处理 sand层边缘
    tmpSandVal = (int)(pnValSandBig +  pnValSandSml );
    //tmpSandVal = tmpVal;

    int sandLvl = (int)floor((float)tmpSandVal/landStep);


    if( (this->lvl==0) || (this->lvl==1) ){

        if( sandLvl >= 0 ){
            this->isSand = true;
        }
    }
    */

    /*
    if( this->lvl == 0 ){
        this->isSand = true;
    }
    */
    
    


}




