/*
 * ====================== MapAltitude.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "MapAltitude.h"

//-------------------- C --------------------//
//#include <cassert>

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"


/* ===========================================================
 *                         set    [2]
 * -----------------------------------------------------------
 */
void MapAltitude::set( float _altiVal_from_gpgpu ){

    tprAssert( (_altiVal_from_gpgpu>=-100.0f) && (_altiVal_from_gpgpu<=100.0f) );
    this->val = static_cast<int>(_altiVal_from_gpgpu);

    //------------------//
    //      lvl
    //------------------//
    int    tmpLvl;
    float waterStep = 7.0f; //- 水下梯度，tmp...
    float landStep  = 14.0f; //- 陆地梯度，tmp...

    if( this->val < 0 ){ //- under water
        tmpLvl = static_cast<int>( floor(_altiVal_from_gpgpu/waterStep) );
        if( tmpLvl < -5 ){ //- 抹平 水域底部
            tmpLvl = -5;
        }
    }else{ //- land
        tmpLvl = static_cast<int>( floor(_altiVal_from_gpgpu/landStep) );
        if( tmpLvl > 2 ){ //- 抹平 陆地高区
            tmpLvl = 2;
        }
    }
    this->lvl = tmpLvl;

}
