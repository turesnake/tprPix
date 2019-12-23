/*
 * ==================== groundGoEntType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GROUND_GO_ENT_TYPE_H
#define TPR_GROUND_GO_ENT_TYPE_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"

enum class GroundGoEntType{
    MapEnt,    
    HalfField,
    Field,       // 相邻异色mapent，所以沿用 多边形 mesh
    SimpleField, // 相邻mapents 皆为同色，本身被简化为一个 正方形（减少计算
};


#endif 

