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
 *                         set    [2]
 * -----------------------------------------------------------
 */
void Altitude::set( float _altiVal_from_gpgpu ){

    assert( (_altiVal_from_gpgpu>=-100.0) && (_altiVal_from_gpgpu<=100.0) );
    this->val = static_cast<int>(_altiVal_from_gpgpu);

    //------------------//
    //      lvl
    //------------------//
    int    tmpLvl;
    float waterStep = 7.0; //- 水下梯度，tmp...
    float landStep  = 14.0; //- 陆地梯度，tmp...

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
    

    //------------------//
    //     isSand
    //------------------//
    this->isSand = false; //- MUST

}




