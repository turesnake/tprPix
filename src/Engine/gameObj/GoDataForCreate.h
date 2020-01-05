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


//------------------- CPP --------------------//
#include <vector>
#include <memory>

//------------------- Engine --------------------//
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "animSubspeciesId.h"
#include "IntVec.h"



class GoDataEntForCreate{
public:
    GoDataEntForCreate()=default;
    //---
    animSubspeciesId_t     subspeciesId {};
    glm::dvec2          dposOff   {}; // gomesh-dposoff based on go
    size_t              windDelayIdx {}; // only used in windClock
};




// 生成一个go实例，需要的基本数据
// - 在 ecoobj 生成阶段，此数据被创建，并永久存储在 ecoobj 中
// - 以 const 指针 的形式，被传递到 job chunk/filed, 以及 具象go类中
// 所以，不用担心此数据的 容量
class GoDataForCreate{
public:
    GoDataForCreate()=default;

    //---
    goSpeciesId_t      goSpeciesId {};
    glm::dvec2      dpos      {}; // go 绝对 dpos
    NineDirection   direction {NineDirection::Center};  //- 角色 动画朝向
    BrokenLvl       brokenLvl {BrokenLvl::Lvl_0};
    
    //---
    bool            isMultiGoMesh {};
    
    //bool            isNeedWind    {}; // 是否需要生成 风吹值,暂时 始终为 true

    //: if  isMultiGoMesh == false： 只有一个元素
    std::vector<std::unique_ptr<GoDataEntForCreate>> goMeshDataUPtrs {};
};




#endif 

