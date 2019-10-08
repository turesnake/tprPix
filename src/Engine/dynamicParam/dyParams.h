/*
 * ======================== dyParams.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.05
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_DY_PARAMS_H
#define TPR_DY_PARAMS_H

//-------------------- CPP --------------------//
#include <vector>
#include <set>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "DyParam.h"
#include "MapAltitude.h"
#include "Density.h"
#include "MapSurfaceRandEnt.h"
#include "AnimLabel.h"

#include "Job_Field.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/mapSurfaces/MapSurfaceSpec.h"



//-- 简陋的临时版 ，传递 field 相关的 常规随机数 --
struct DyParams_Field{
    size_t       uWeight;
	MapAltitude  mapEntAlti;
	Density      mapEntDensity;
    std::vector<AnimLabel> animLabels; //- maybe empty
};



struct DyParams_MapSurface{
    MapSurfaceLowSpec       spec; // "mapSurfaceLow_test"
    mapSurface::RandEntLvl  lvl; // "Field_1f1"
    size_t                  randUVal; // field.uWeight
};



struct DyParams_GroundGo{
    size_t      fieldUWeight;
    const Job_Field  *job_fieldPtr;
};



#endif 

