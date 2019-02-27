/*
 * =================== MapEntInBuild.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section build态 才需要的 mapent数据
 * ----------------------------
 */
#ifndef _TPR_MAP_ENT_IN_BUILD_H
#define _TPR_MAP_ENT_IN_BUILD_H


//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "EcoSysType.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


//- section-build 阶段的 mapent 数据结构
class MapEntInBuild{
public:
    IntVec2     mpos {0,0}; //- 在 （256，256）tex 中的 坐标
    u8_t        alti {0}; //- 高度 [0,15] 
    //u8_t      mask  {0};
    //u8_t      road  {0};
    u8_t        isLand {1}; //- 1:land, 0:waters
    EcoSysType  ecoSysType { EcoSysType::Forst }; 
};


}//----------------- namespace: sectionBuild: end -------------------//
#endif 

