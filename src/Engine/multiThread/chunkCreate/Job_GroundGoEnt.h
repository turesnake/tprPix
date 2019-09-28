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


enum class Job_GroundGoEntType{
    Mapent,    
    HalfField,
    Field,       // 相邻异色mapent，所以沿用 多边形 mesh
    SimpleField, // 相邻mapents 皆为同色，本身被简化为一个 正方形（减少计算
};


//-- 每个实例，都会变成 GroundGo 的一个 mesh
class Job_GroundGoEnt{
public:
    Job_GroundGoEnt(   Job_GroundGoEntType    type_,
                    const glm::vec2     fposOff_,
                    colorTableId_t      id_,
                    double              uWeight_ ):
        groundType(type_),
        fposOff(fposOff_),
        colorTableId(id_),
        uWeight(uWeight_)
        {}

    Job_GroundGoEntType     groundType;
    glm::vec2               fposOff; // base on field-midDPos
    //---
    //sectionKey_t          ecoObjKey;
    colorTableId_t          colorTableId;
    double                  uWeight; //[0.0, 97.0]
};

#endif 

