/*
 * ========================= field_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>
#include <mutex>

//-------------------- Engine --------------------//
#include "esrc_field.h"
#include "config.h"
#include "chunkKey.h"

//#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

namespace{//------------ namespace --------------//

    std::unordered_map<fieldKey_t,MapField> fields {};

    std::mutex  fieldsMutex;

    //===== funcs =====//
    MapField *insert_and_init_new_field( fieldKey_t _fieldKey );

}//---------------- namespace end --------------//


/* ===========================================================
 *            atom_try_to_insert_and_init_the_field_ptr
 * -----------------------------------------------------------
 * 检测是否存在，若不存在，生成之
 * ----
 * 目前被 check_and_build_sections_3.cpp -> build_one_chunk_3() 调用
 */
void atom_try_to_insert_and_init_the_field_ptr( const IntVec2 &_fieldMPos ){

    fieldKey_t fieldKey = fieldMPos_2_fieldKey( _fieldMPos );
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(fieldsMutex);
        if( esrc::fields.find(fieldKey) == esrc::fields.end() ){
            insert_and_init_new_field(fieldKey);
        }
    }//----- 这段时间看起来有点长，毕竟包含了 field.init() 
}




/* ===========================================================
 *                 atom_get_fieldPtr
 * -----------------------------------------------------------
 * ---
 * 目前被：
 *     create_a_go_in_field.cpp -> create_a_go_in_field()
 *     Chunk.cpp -> colloect_nearFour_fieldDatas() 
 *     使用
 */
MapField *atom_get_fieldPtr( fieldKey_t _fieldKey ){
    MapField *ptr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(fieldsMutex);
        assert( esrc::fields.find(_fieldKey) != esrc::fields.end() );//- must exist
    ptr = &(esrc::fields.at(_fieldKey));
    }
    return ptr;
}



namespace{//------------ namespace --------------//

/* ===========================================================
 *                  insert_and_init_new_field   [1]
 * -----------------------------------------------------------
 */
MapField *insert_and_init_new_field( fieldKey_t _fieldKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapField  field {};
    field.init( fieldKey_2_mpos(_fieldKey) );
        assert( esrc::fields.find(_fieldKey) == esrc::fields.end() ); //- must not exist
    esrc::fields.insert({ _fieldKey, field }); //- copy
    //-----
    return static_cast<MapField*>( &(esrc::fields.at(_fieldKey)) );
}

}//---------------- namespace end --------------//



}//---------------------- namespace: esrc -------------------------//

