/*
 * ===================== DiskGameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    GameObj 各种数据 结构
 * ----------------------------
 */
#ifndef TPR_DISK_GAME_OBJ_H_
#define TPR_DISK_GAME_OBJ_H_

//------------------- Engine --------------------//
#include "IntVec.h"
#include "GameObjType.h"


class DiskGameObj{
public:
    goid_t         goid     {}; //- u64   
    goSpecId_t     goSpecId {}; //- u32
    IntVec2        mpos     {}; //- int, int
    //...

};


#endif 

