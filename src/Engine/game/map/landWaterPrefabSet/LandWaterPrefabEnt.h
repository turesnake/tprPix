/*
 * ==================== LandWaterPrefabEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_LAND_WATER_PREFAB_ENT_H_
#define _TPR_LAND_WATER_PREFAB_ENT_H_

//-------------------- Engine --------------------//
#include "IntVec.h"

//-- 预制件中 单个像素／mapent 的信息 --
class LandWaterPrefabEnt{
public:
    LandWaterPrefabEnt() = default;
    LandWaterPrefabEnt( int _x, int _y, bool _is_major ){
        lMPosOff.x = _x;
        lMPosOff.y = _y;
        is_major = _is_major;
    }

    IntVec2  lMPosOff {};     //- 相对于 预制件中心 的 mpos 偏移
    bool     is_major {true}; //- 主色／副色 
};


#endif 

