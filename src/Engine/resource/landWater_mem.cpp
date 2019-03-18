/*
 * ========================= landWater_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源


#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                 get_landWaterPrefabEdgeId
 * -----------------------------------------------------------
 */
landWaterPrefabEdgeId_t get_landWaterPrefabEdgeId( chunkKey_t _chunkKey ){

    if( landWaterPrefabEdgeIds.find(_chunkKey) == landWaterPrefabEdgeIds.end() ){
        cout << "landWaterPrefabEdgeIds: cant find chunkKey = " << _chunkKey
            << endl;
    }
    assert( landWaterPrefabEdgeIds.find(_chunkKey) != landWaterPrefabEdgeIds.end() );
    return landWaterPrefabEdgeIds.at(_chunkKey);
}

/* ===========================================================
 *               get_landWaterPrefabCornerId
 * -----------------------------------------------------------
 */
landWaterPrefabCornerId_t get_landWaterPrefabCornerId( sectionKey_t _sectionKey ){

    if( landWaterPrefabCornerIds.find(_sectionKey) == landWaterPrefabCornerIds.end() ){
        cout << "landWaterPrefabCornerIds : cant find sectionKey = " << _sectionKey
            << endl;
    }

    assert( landWaterPrefabCornerIds.find(_sectionKey) != landWaterPrefabCornerIds.end() );
    return landWaterPrefabCornerIds.at(_sectionKey);
}

/* ===========================================================
 *               insert_new_landWaterEntSet
 * -----------------------------------------------------------
 */
std::vector<LandWaterEnt> &insert_new_landWaterEntSet( chunkKey_t _key ){
    assert( landWaterEntSets.find(_key) == landWaterEntSets.end() );
    landWaterEntSets.insert({ _key, std::vector<LandWaterEnt>{} });
    return landWaterEntSets.at(_key);
}

/* ===========================================================
 *               get_landWaterEntSet
 * -----------------------------------------------------------
 */
std::vector<LandWaterEnt> &get_landWaterEntSet( chunkKey_t _key ){
    assert( landWaterEntSets.find(_key) != landWaterEntSets.end() );
    return landWaterEntSets.at(_key);
}

/* ===========================================================
 *               erase_landWaterEntSet
 * -----------------------------------------------------------
 */
void erase_landWaterEntSet( chunkKey_t _key ){
    assert( landWaterEntSets.find(_key) != landWaterEntSets.end() );
    assert( landWaterEntSets.erase(_key) == 1 );
}


}//---------------------- namespace: esrc -------------------------//

