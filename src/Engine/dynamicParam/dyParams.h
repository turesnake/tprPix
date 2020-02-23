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
#include <cstdint> // uint8_t


//-------------------- Engine --------------------//
#include "DyParam.h"
#include "MapAltitude.h"
#include "Density.h"
#include "AnimLabel.h"
#include "GoDataForCreate.h"

#include "Job_Field.h"



struct DyParams_Blueprint{    
    size_t                  mapEntUWeight {}; // 其实是 mapEntKey 
    const GoDataForCreate   *goDataPtr {};
};


struct DyParams_GroundGo{
    size_t      fieldUWeight  {};
    const Job_Field  *job_fieldPtr {};
};

/*
struct DyParams_RiverBank{    
    size_t                  mapEntUWeight {}; // 其实是 mapEntKey 
    //...
};
*/




#endif 

