/*
 * ========================= field_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk 内存态
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
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
    return (MapField*)&(esrc::fields.at(_fieldKey));
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
    return (MapField*)&(esrc::fields.at(fieldKey));
}


/* ===========================================================
 *                find_or_insert_the_field
 * -----------------------------------------------------------
 * 若存在，取之，若不存在，生成之
 */
MapField *find_or_insert_the_field( fieldKey_t _fieldKey ){

    if( esrc::fields.find(_fieldKey) == esrc::fields.end() ){
        return esrc::insert_new_field( _fieldKey );
    }else{
        return (MapField*)&(esrc::fields.at(_fieldKey));
    }
}





}//---------------------- namespace: esrc -------------------------//

