/*
 * ================= esrc_mapSurfaceRand.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_mapSurfaceRand.h"


//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>
#include <memory>
#include "esrc_state.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

namespace msr_inn {//-------- namespace: msr_inn --------------//

    std::unique_ptr<mapSurface::RandEntSet> randEntSetUPtr {nullptr};
    std::unique_ptr<mapSurface::RandMeshSet> randMeshSetUPtr {nullptr};

}//------------- namespace: msr_inn end --------------//


void init_mapSurfaceRandSet(){
    msr_inn::randEntSetUPtr = std::make_unique<mapSurface::RandEntSet>();
    msr_inn::randEntSetUPtr->load_mapSurfaceRandSets();
    //---
    msr_inn::randMeshSetUPtr = std::make_unique<mapSurface::RandMeshSet>();

    esrc::insertState("mapSurfaceRandSet");
}


mapSurface::randEntId_t apply_a_mapSurfaceRandEntId( double randVal_ ){
    return msr_inn::randEntSetUPtr->apply_a_randEntId(randVal_);
}


const std::pair<mapSurface::RandEntLvl,glm::dvec2> & 
get_mapSurfaceRandEntData(mapSurface::randEntId_t entId_, size_t fieldIdx_in_chunk_  ){
    return msr_inn::randEntSetUPtr->get_randEntData(entId_, fieldIdx_in_chunk_);
}


mapSurface::RandMesh &apply_new_mapSurfaceRandMeshData( mapSurface::RandEntLvl lvl_ )noexcept{
    return msr_inn::randMeshSetUPtr->apply_new_randMeshData( lvl_ );
}


const mapSurface::RandMesh &
get_a_mapSurfaceRandMeshData( mapSurface::RandEntLvl lvl_, double randVal_ )noexcept{
    return msr_inn::randMeshSetUPtr->get_a_randMeshData( lvl_, randVal_ );
}




}//---------------------- namespace: esrc -------------------------//

