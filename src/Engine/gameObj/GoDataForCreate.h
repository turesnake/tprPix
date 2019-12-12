/*
 * ===================== GoDataForCreate.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_DATA_FOR_CREATE_H
#define TPR_GO_DATA_FOR_CREATE_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//------------------- Engine --------------------//
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "animSubspeciesId.h"
#include "IntVec.h"


// 生成一个go实例，需要的基本数据
// - 在 ecoobj 生成阶段，此数据被创建，并永久存储在 ecoobj 中
// - 以 const 指针 的形式，被传递到 job chunk/filed, 以及 具象go类中
// 所以，不用担心此数据的 容量
class GoDataForCreate{
public:
    GoDataForCreate()=default;

    //---
    goSpecId_t              goSpecId {};
    animSubspeciesId_t      subspecId {};  // 亚种数据，直接在 ecoobj 阶段生成，依靠 mapEntKey,而不是 mapEnt.uWeight 
    NineDirection           direction {NineDirection::Mid};  //- 角色 动画朝向
    BrokenLvl               brokenLvl {BrokenLvl::Lvl_0};
    glm::dvec2              dpos      {}; // 绝对地址




};




#endif 

