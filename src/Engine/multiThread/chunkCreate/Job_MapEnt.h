/*
 * ========================== Job_MapEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_MAP_ENT_H
#define TPR_JOB_MAP_ENT_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>
#include <set>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "config.h"
#include "IntVec.h"
#include "sectionKey.h"
#include "chunkKey.h"
#include "fieldKey.h"
#include "colorTableId.h"
#include "Density.h"
#include "MapAltitude.h"
#include "GoSpecData.h"

#include "Job_GroundGoEnt.h"


class MemMapEnt;


class Job_MapEnt{
public:
    
    void init( IntVec2 mpos_, chunkKey_t chunkKey_ )noexcept;
    //====== vals ======//
    IntVec2    mpos         {};
    IntVec2    midPPos      {}; // 中间点

    chunkKey_t          chunkKey  {};
    sectionKey_t        ecoObjKey {};
    colorTableId_t      colorTableId {}; // same as ecoObj.colorTableId
    Density             density {};
    MapAltitude         alti {};

    //double  originPerlin {}; // [-1.0, 1.0]
    size_t  uWeight      {}; // [0, 9999]

    bool  isBorder {false}; //- 是否为 eco边缘go

                             // 在未来，将被拓展为 一个 具体的数字，表示自己离 border 的距离（mapents）...
                              

    void write_2_mapEnt( MemMapEnt &entRef_ )const noexcept;

private:
    double calc_pixAlti( IntVec2 pixPPos_ )noexcept;

    //===== static =====//
    static const IntVec2 pixesPerHalfMapent;
}; 



#endif 

