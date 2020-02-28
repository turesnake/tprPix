/*
 * ====================== Job_GroundGoEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_GROUND_GO_ENT_H
#define TPR_JOB_GROUND_GO_ENT_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
//#include <vector>

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "colorTableId.h"
#include "groundGoEntType.h"

//-- 每个实例，都会变成 GroundGo 的一个 mesh
class Job_GroundGoEnt{
public:
    Job_GroundGoEnt(   GroundGoEntType  type_,
                    const glm::dvec2    dposOff_,
                    colorTableId_t      id_,
                    size_t              uWeight_ ):
        groundType(type_),
        dposOff(dposOff_),
        colorTableId(id_),
        uWeight(uWeight_)
        {}

    GroundGoEntType         groundType;
    glm::dvec2              dposOff; // base on field-midDPos
    //---
    colorTableId_t          colorTableId; // 不再需要这个值

    size_t                  uWeight; //[0.0, 9999]
};




#endif 

