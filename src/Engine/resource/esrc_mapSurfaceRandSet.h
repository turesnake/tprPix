/*
 * ================== esrc_mapSurfaceRandSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_MAP_SURFACE_RAND_SET_H
#define TPR_ESRC_MAP_SURFACE_RAND_SET_H

//-------------------- Engine --------------------//
#include "MapSurfaceRandSet.h"

namespace esrc {//------------------ namespace: esrc -------------------------//  


void init_mapSurfaceRandSet();



mapSurfaceRandEntId_t apply_a_mapSurfaceRandEntId( double randVal_);


MapSurfaceRandLvl get_mapSurfaceRandLvl(mapSurfaceRandEntId_t entId_, 
                                        size_t fieldIdx_in_chunk_  );



}//---------------------- namespace: esrc -------------------------//
#endif

