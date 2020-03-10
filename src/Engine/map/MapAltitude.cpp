/*
 * ====================== MapAltitude.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "MapAltitude.h"



void MapAltitude::init( double altiVal_from_gpgpu_ ){

    tprAssert( (altiVal_from_gpgpu_>=-100.0) && (altiVal_from_gpgpu_<=100.0) );
    this->val = static_cast<int>(floor(altiVal_from_gpgpu_));

    //------------------//
    //      lvl
    //------------------//
    int    tmpLvl    {};
    double waterStep  { 7.0 }; //- 水下梯度，tmp...
    double landStep   { 14.0 }; //- 陆地梯度，tmp...

    if( this->val < 0 ){ //- under water
        tmpLvl = static_cast<int>( floor(altiVal_from_gpgpu_/waterStep) );
        if( tmpLvl < -5 ){ //- 抹平 水域底部
            tmpLvl = -5;
        }
    }else{ //- land
        tmpLvl = static_cast<int>( floor(altiVal_from_gpgpu_/landStep) );
        if( tmpLvl > 2 ){ //- 抹平 陆地高区
            tmpLvl = 2;
        }
    }
    this->lvl = tmpLvl;
}
