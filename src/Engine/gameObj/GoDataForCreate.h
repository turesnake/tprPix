/*
 * ===================== GoDataForCreate.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_DATA_FOR_CREATE_H
#define TPR_GO_DATA_FOR_CREATE_H


//------------------- Engine --------------------//
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "animSubspeciesId.h"
#include "IntVec.h"


// 生成一个go实例，需要的基本数据
// 它们将印象一个 go 的外貌
// 比如存储在 ecoObj 中的 mp-go 数据
class GoDataForCreate{
public:
    GoDataForCreate()=default;
    //---
    goSpecId_t              goSpecId {};
    animSubspeciesId_t      subspecId {};  // 亚种数据，直接在 ecoobj 阶段生成，依靠 mapEntKey,而不是 mapEnt.uWeight 
    NineDirection           direction {NineDirection::Mid};  //- 角色 动画朝向
    BrokenLvl               brokenLvl {BrokenLvl::Lvl_0};
    IntVec2                 mpos    {};
};




#endif 

