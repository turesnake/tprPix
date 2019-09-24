/*
 * ======================= UBO_Seeds.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UBO_SEEDS_H
#define TPR_UBO_SEEDS_H

//-------------------- Engine --------------------//
#include "FloatVec.h"


// [once]
struct UBO_Seeds{
    FloatVec2 altiSeed_pposOffSeaLvl {};
    FloatVec2 altiSeed_pposOffBig {};
    FloatVec2 altiSeed_pposOffMid {};
    FloatVec2 altiSeed_pposOffSml {};
    //....
};


void write_ubo_Seeds();

#endif 

