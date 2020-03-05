/*
 * ===================== DiskGameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    GameObj 各种数据 结构
 * ----------------------------
 */
#ifndef TPR_DISK_GAME_OBJ_H
#define TPR_DISK_GAME_OBJ_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- Engine --------------------//
#include "IntVec.h"
#include "GameObjType.h"
#include "goLabelId.h"
#include "NineDirection.h"
#include "BrokenLvl.h"


class DiskGameObj{
public:
    goid_t         goid     {}; //- u64   
    goSpeciesId_t  goSpeciesId {}; //- u32
    goLabelId_t    goLabelId {}; // u64
    glm::dvec2     dpos     {}; //- double, double
    size_t         goUWeight {}; // u64
    NineDirection  dir {}; // int
    BrokenLvl      brokenLvl {}; // int

};




#endif 

