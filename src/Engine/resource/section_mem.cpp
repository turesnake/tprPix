/*
 * ========================= section_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section 内存态  [tmp]
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "esrc_section.h"

#include "config.h"
#include "sectionKey.h"

#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                insert_new_section   [1]
 * -----------------------------------------------------------
 * param: _anyMPos --  目标 section 区域中的任何一个 mapent.mpos
 * 初始化以下数据：
 *  - sectionKey
 *  - mcpos
 */
Section *insert_new_section( const IntVec2 &_anyMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    Section  section {};
    section.set_by_anyMPos( _anyMPos ); 
    sectionKey_t key = section.sectionKey;
        assert( esrc::sections.find(key) == esrc::sections.end() );//- must not exist
    esrc::sections.insert({ key, section }); //- copy
    //-----
    return static_cast<Section*>( &(esrc::sections.at(key)) );
}


/* ===========================================================
 *                insert_new_section   [2]
 * -----------------------------------------------------------
 * param: _sectionkey --  目标 section key
 * 初始化以下数据：
 *  - sectionKey
 *  - mcpos
 */
Section *insert_new_section( sectionKey_t _sectionkey ){

    // ***| INSERT FIRST, INIT LATER  |***
    Section  section {};
    section.set_by_sectionKey( _sectionkey );
        assert( esrc::sections.find(_sectionkey) == esrc::sections.end() );//- must not exist
    esrc::sections.insert({ _sectionkey, section }); //- copy
    //-----
    return static_cast<Section*>( &(esrc::sections.at(_sectionkey)) );
}


}//---------------------- namespace: esrc -------------------------//

