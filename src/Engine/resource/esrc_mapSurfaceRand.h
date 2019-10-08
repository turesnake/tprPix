/*
 * ================== esrc_mapSurfaceRand.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_MAP_SURFACE_RAND_H
#define TPR_ESRC_MAP_SURFACE_RAND_H

//-------------------- Engine --------------------//
#include "MapSurfaceRandEnt.h"
#include "MapSurfaceRandMesh.h"

namespace esrc {//------------------ namespace: esrc -------------------------//  


void init_mapSurfaceRandSet();



mapSurface::randEntId_t apply_a_mapSurfaceRandEntId( size_t randUVal_);


const std::pair<mapSurface::RandEntLvl,glm::dvec2> & 
get_mapSurfaceRandEntData(mapSurface::randEntId_t entId_, size_t fieldIdx_in_chunk_  );


mapSurface::RandMesh &apply_new_mapSurfaceRandMeshData( mapSurface::RandEntLvl lvl_ )noexcept;

const mapSurface::RandMesh &
get_a_mapSurfaceRandMeshData( mapSurface::RandEntLvl lvl_, size_t randUVal_ )noexcept;



}//---------------------- namespace: esrc -------------------------//
#endif

