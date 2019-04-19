/*
 * ========================= esrc_ecoSysInMap.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_ECOSYSINMAP_H_
#define _TPR_ESRC_ECOSYSINMAP_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "EcoSysInMap.h"
#include "IntVec.h"
#include "sectionKey.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


inline std::unordered_map<sectionKey_t, EcoSysInMap> ecoSysesInMap {};

EcoSysInMap *insert_new_ecoSysInMap( const IntVec2 &_sectionMPos );
EcoSysInMap *insert_new_ecoSysInMap( sectionKey_t _sectionkey );

inline bool find_from_ecoSysesInMap( sectionKey_t _sectionKey ){
    return (esrc::ecoSysesInMap.find(_sectionKey) != esrc::ecoSysesInMap.end());
}

inline EcoSysInMap *get_ecoSysInMapPtr( sectionKey_t _sectionkey ){
        assert( ecoSysesInMap.find(_sectionkey) != ecoSysesInMap.end() );//- tmp
    return  &(ecoSysesInMap.at(_sectionkey));
}

inline EcoSysInMap &get_ecoSysInMapRef( sectionKey_t _sectionkey ){
        assert( ecoSysesInMap.find(_sectionkey) != ecoSysesInMap.end() );//- tmp
    return ecoSysesInMap.at(_sectionkey);
}


}//---------------------- namespace: esrc -------------------------//
#endif

