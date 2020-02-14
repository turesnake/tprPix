/*
 * ========================== EcoObj_ReadOnly.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ECO_OBJ_READ_ONLY_H
#define TPR_ECO_OBJ_READ_ONLY_H

//-------------------- Engine --------------------//
#include "EcoObj.h"
#include "EcoObjBorder.h"


//-- 仅用于 atom 函数 值传递 --
class EcoObj_ReadOnly{
public:
    sectionKey_t   sectionKey       {};
    colorTableId_t colorTableId     {};
    double         densitySeaLvlOff {};
    //double         uWeight          {};
    const std::vector<double> *densityDivideValsPtr {};
    const EcoObjBorder *ecoObjBorderPtr {nullptr};
};

#endif 

