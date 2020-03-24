/*
 * ======================= esrc_field.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
//-------------------- CPP --------------------//
#include <mutex>
#include <shared_mutex> //- c++17 读写锁


//-------------------- Engine --------------------//
#include "esrc_field.h"
#include "chunkKey.h"

#include "esrc_ecoObj.h"
#include "esrc_gameObj.h" 
#include "esrc_chunk.h" 
#include "esrc_state.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace field_inn {//------------ namespace: field_inn --------------//

    std::unordered_map<fieldKey_t,std::unique_ptr<MapField>> fields {};
 
}//---------------- namespace: field_inn end --------------//


void init_fields(){
    field_inn::fields.reserve(10000);
    esrc::insertState("field");
}



void move_fieldUPtrs( std::unordered_map<fieldKey_t, std::unique_ptr<MapField>> &fields_ ){

    for( auto &[iKey, iUPtr] : fields_ ){
        auto [insertIt, insertBool] = field_inn::fields.emplace( iKey, std::move(iUPtr) );
        tprAssert( insertBool );
    }
}



void erase_all_fields_in_chunk( IntVec2 chunkMPos_ ){

    IntVec2    tmpFieldMPos {};
    fieldKey_t tmpFieldKey {};
    size_t     eraseNum {};

    for( int h=0; h<FIELDS_PER_CHUNK<>; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK<>; w++ ){
            tmpFieldMPos = IntVec2{ chunkMPos_.x + w*ENTS_PER_FIELD<>,
                                    chunkMPos_.y + h*ENTS_PER_FIELD<> };
            tmpFieldKey = fieldMPos_2_fieldKey( tmpFieldMPos );

            eraseNum = field_inn::fields.erase(tmpFieldKey);
            tprAssert( eraseNum == 1 );
        }
    }
}


std::optional<const MapField*> get_fieldPtr( fieldKey_t fieldKey_ ){
    if( field_inn::fields.find(fieldKey_) == field_inn::fields.end() ){
        return std::nullopt;
    }   
    return { field_inn::fields.at(fieldKey_).get() };
}


}//---------------------- namespace: esrc end -------------------------//

