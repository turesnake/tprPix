/*
 * ========================= ecoSysesInMap_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ecoSysInMap 内存态
 * ----------------------------
 */
//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <mutex>

//-------------------- Engine --------------------//
#include "esrc_ecoSysInMap.h"

#include "config.h"
#include "sectionKey.h"

//#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

namespace{//------------ namespace --------------//

    std::unordered_map<sectionKey_t, EcoSysInMap> ecoSysesInMap {};

    std::mutex  ecoSysInMapMutex;


}//---------------- namespace end --------------//


/* ===========================================================
 *               atom_insert_new_ecoSysInMap
 * -----------------------------------------------------------
 * param: _sectionMPos --  seection左下角 mcpos
 * 已初始化数据：
 *  - sectionKey
 *  - mcpos
 */
EcoSysInMap *atom_insert_new_ecoSysInMap( const IntVec2 &_sectionMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSysInMap *ptr;
    EcoSysInMap  ecoSysInMap {};
    ecoSysInMap.set_by_sectionMPos( _sectionMPos );
    sectionKey_t  key = ecoSysInMap.sectionKey;

    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(key) == esrc::ecoSysesInMap.end() );//- must not exist
        esrc::ecoSysesInMap.insert({ key, ecoSysInMap }); //- copy
        ptr = &(esrc::ecoSysesInMap.at(key));
    }
    return ptr;
}


/* ===========================================================
 *               atom_find_from_ecoSysesInMap
 * -----------------------------------------------------------
 */
bool atom_find_from_ecoSysesInMap( sectionKey_t _sectionKey ){
    bool ret;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
        ret = esrc::ecoSysesInMap.find(_sectionKey) != esrc::ecoSysesInMap.end();
    }
    return ret;
}

/* ===========================================================
 *               atom_get_ecoSysInMapPtr
 * -----------------------------------------------------------
 */
EcoSysInMap *atom_get_ecoSysInMapPtr( sectionKey_t _sectionkey ){
    EcoSysInMap *ptr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(_sectionkey) != esrc::ecoSysesInMap.end() );//- tmp
        ptr = &(esrc::ecoSysesInMap.at(_sectionkey));
    }
    return ptr;
}


}//---------------------- namespace: esrc -------------------------//

