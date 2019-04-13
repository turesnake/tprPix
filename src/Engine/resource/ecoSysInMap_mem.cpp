/*
 * ========================= ecoSysInMap_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ecoSysInMap 内存态
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "config.h"
#include "sectionKey.h"

#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//



/* ===========================================================
 *               insert_new_ecoSysInMap
 * -----------------------------------------------------------
 * param: _sectionMPos --  seection左下角 mcpos
 * 已初始化数据：
 *  - sectionKey
 *  - mcpos
 */
EcoSysInMap *insert_new_ecoSysInMap( const IntVec2 &_sectionMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSysInMap  ecoSysInMap {};
    ecoSysInMap.set_by_sectionMPos( _sectionMPos );
    sectionKey_t  key = ecoSysInMap.sectionKey;
        assert( esrc::ecoSysesInMap.find(key) == esrc::ecoSysesInMap.end() );//- must not exist
    esrc::ecoSysesInMap.insert({ key, ecoSysInMap }); //- copy
    //----
    return static_cast<EcoSysInMap*>( &(esrc::ecoSysesInMap.at(key)) );
}


/* ===========================================================
 *               insert_new_ecoSysInMap
 * -----------------------------------------------------------
 * param: _sectionkey --  seection key
 * 已初始化数据：
 *  - sectionKey
 *  - mcpos
 */
EcoSysInMap *insert_new_ecoSysInMap( sectionKey_t _sectionkey ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSysInMap  ecoSysInMap {};
    ecoSysInMap.set_by_sectionKey( _sectionkey );
        assert( esrc::ecoSysesInMap.find(_sectionkey) == esrc::ecoSysesInMap.end() );//- must not exist
    esrc::ecoSysesInMap.insert({ _sectionkey, ecoSysInMap }); //- copy
    //----
    return static_cast<EcoSysInMap*>( &(esrc::ecoSysesInMap.at(_sectionkey)) );
}




}//---------------------- namespace: esrc -------------------------//

