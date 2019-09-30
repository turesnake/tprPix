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
                    const glm::vec2     fposOff_,
                    colorTableId_t      id_,
                    double              uWeight_ ):
        groundType(type_),
        fposOff(fposOff_),
        colorTableId(id_),
        uWeight(uWeight_)
        {}

    GroundGoEntType         groundType;
    glm::vec2               fposOff; // base on field-midDPos
    //---
    //sectionKey_t          ecoObjKey;
    colorTableId_t          colorTableId;
    double                  uWeight; //[0.0, 97.0]
};

#endif 

