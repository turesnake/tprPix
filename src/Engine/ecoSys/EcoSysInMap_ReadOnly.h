/*
 * ========================== EcoSysInMap_ReadOnly.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_ECOSYS_IN_MAP_READ_ONLY_H_
#define _TPR_ECOSYS_IN_MAP_READ_ONLY_H_

//-------------------- Engine --------------------//
#include "EcoSysInMap.h"


//-- 仅用于 atom 函数 值传递 --
class EcoSysInMap_ReadOnly{
public:
    sectionKey_t   sectionKey       {};
    float          densitySeaLvlOff {};
    const std::vector<float> *densityDivideValsPtr {};
};

#endif 

