/*
 * ========================== EcoObj_ReadOnly.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ECO_OBJ_READ_ONLY_H_
#define TPR_ECO_OBJ_READ_ONLY_H_

//-------------------- Engine --------------------//
#include "EcoObj.h"


//-- 仅用于 atom 函数 值传递 --
class EcoObj_ReadOnly{
public:
    sectionKey_t   sectionKey       {};
    float          densitySeaLvlOff {};
    const std::vector<float> *densityDivideValsPtr {};
};

#endif 

