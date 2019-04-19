/*
 * ========================= esrc_field.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_FIELD_H_
#define _TPR_ESRC_FIELD_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "MapField.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


inline std::unordered_map<fieldKey_t,MapField> fields {};

MapField *insert_new_field( fieldKey_t _fieldKey );
MapField *insert_new_field( const IntVec2 &_anyMPos );
MapField &insert_new_field_ref( fieldKey_t _fieldKey );

MapField *find_or_insert_the_field_ptr( fieldKey_t _fieldKey );
MapField &find_or_insert_the_field_ref( fieldKey_t _fieldKey );


inline  MapField *get_fieldPtr( fieldKey_t _fieldKey ){
        assert( esrc::fields.find(_fieldKey) != esrc::fields.end() );//- must exist
    return static_cast<MapField*>( &(esrc::fields.at(_fieldKey)) );
}


}//---------------------- namespace: esrc -------------------------//
#endif

