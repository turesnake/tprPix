/*
 * ======================= esrc_field.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- CPP --------------------//
#include <unordered_map>
#include <mutex>
#include <shared_mutex> //- c++17 读写锁
#include <set>
#include <memory>


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_field.h"
#include "config.h"
#include "chunkKey.h"

#include "esrc_ecoObj.h"
#include "esrc_gameObj.h" 
#include "esrc_chunk.h" 
#include "esrc_mapSurfaceRand.h"
#include "esrc_state.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc_all.h"



#include "tprDebug.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace field_inn {//------------ namespace: field_inn --------------//

    std::unordered_map<fieldKey_t,std::unique_ptr<MapField>> fields {};
 
}//---------------- namespace: field_inn end --------------//


void init_fields(){
    field_inn::fields.reserve(10000);
    esrc::insertState("field");
}



void move_fieldUPtrs( std::unordered_map<fieldKey_t, std::unique_ptr<MapField>> &fields_ ){

    for( auto &pair : fields_ ){
        auto outPair = field_inn::fields.insert({ pair.first, std::move(pair.second) });
        tprAssert( outPair.second );
    }
}



void erase_all_fields_in_chunk( IntVec2 chunkMPos_ ){

    IntVec2    tmpFieldMPos {};
    fieldKey_t tmpFieldKey {};
    size_t     eraseNum {};

    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){
            tmpFieldMPos.set(   chunkMPos_.x + w*ENTS_PER_FIELD,
                                chunkMPos_.y + h*ENTS_PER_FIELD );
            tmpFieldKey = fieldMPos_2_fieldKey( tmpFieldMPos );

            eraseNum = field_inn::fields.erase(tmpFieldKey);
            tprAssert( eraseNum == 1 );
        }
    }
}


const MapField &get_field( fieldKey_t fieldKey_ ){
    tprAssert( field_inn::fields.find(fieldKey_) != field_inn::fields.end() );
    return *(field_inn::fields.at( fieldKey_ ));
}


}//---------------------- namespace: esrc end -------------------------//

