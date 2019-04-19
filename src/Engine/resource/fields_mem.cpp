/*
 * ========================= field_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- Engine --------------------//
#include "esrc_field.h"

#include "config.h"
#include "chunkKey.h"

//#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//



/* ===========================================================
 *                  insert_new_field   [1]
 * -----------------------------------------------------------
 */
MapField *insert_new_field( fieldKey_t _fieldKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapField  field {};
    field.init( fieldKey_2_mpos(_fieldKey) );
        assert( esrc::fields.find(_fieldKey) == esrc::fields.end() ); //- must not exist
    esrc::fields.insert({ _fieldKey, field }); //- copy
    //-----
    return static_cast<MapField*>( &(esrc::fields.at(_fieldKey)) );
}

MapField &insert_new_field_ref( fieldKey_t _fieldKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapField  field {};
    field.init( fieldKey_2_mpos(_fieldKey) );
        assert( esrc::fields.find(_fieldKey) == esrc::fields.end() ); //- must not exist
    esrc::fields.insert({ _fieldKey, field }); //- copy
    //-----
    return esrc::fields.at(_fieldKey);
}

/* ===========================================================
 *                  insert_new_field   [2]
 * -----------------------------------------------------------
 */
MapField *insert_new_field( const IntVec2 &_anyMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    fieldKey_t  fieldKey = anyMPos_2_fieldKey( _anyMPos );
    MapField  field {};
    field.init( fieldKey_2_mpos(fieldKey) );
        assert( esrc::fields.find(fieldKey) == esrc::fields.end() ); //- must not exist
    esrc::fields.insert({ fieldKey, field }); //- copy
    //-----
    return static_cast<MapField*>( &(esrc::fields.at(fieldKey)) );
}


/* ===========================================================
 *                find_or_insert_the_field [2]
 * -----------------------------------------------------------
 * 若存在，取之，若不存在，生成之
 */
MapField *find_or_insert_the_field_ptr( fieldKey_t _fieldKey ){

    return (esrc::fields.find(_fieldKey) == esrc::fields.end()) ?
                esrc::insert_new_field(_fieldKey) :
                &(esrc::fields.at(_fieldKey));
}

MapField &find_or_insert_the_field_ref( fieldKey_t _fieldKey ){

    return (esrc::fields.find(_fieldKey) == esrc::fields.end()) ?
                esrc::insert_new_field_ref(_fieldKey) :
                esrc::fields.at(_fieldKey);
}





}//---------------------- namespace: esrc -------------------------//

