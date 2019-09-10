/*
 * ================= esrc_mapSurfaceRandSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_mapSurfaceRandSet.h"


//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>
#include <memory>


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace mapSurfaceRandSet_inn {//-------- namespace: mapSurfaceRandSet_inn --------------//

    std::unique_ptr<MapSurfaceRandSet> mapSurfaceRandSetUPtr {nullptr};

}//------------- namespace: mapSurfaceRandSet_inn end --------------//



void init_mapSurfaceRandSet(){
    mapSurfaceRandSet_inn::mapSurfaceRandSetUPtr = std::make_unique<MapSurfaceRandSet>();
    mapSurfaceRandSet_inn::mapSurfaceRandSetUPtr->load_mapSurfaceRandSets();
}



mapSurfaceRandEntId_t apply_a_mapSurfaceRandEntId( double randVal_ ){
    return mapSurfaceRandSet_inn::mapSurfaceRandSetUPtr->apply_a_mapSurfaceRandEntId(randVal_);
}




}//---------------------- namespace: esrc -------------------------//

