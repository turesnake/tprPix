/*
 * ========================= esrc_ecoSysInMap.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_ECOSYSINMAP_H_
#define _TPR_ESRC_ECOSYSINMAP_H_

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "EcoSysInMap.h"
#include "IntVec.h"
#include "sectionKey.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


EcoSysInMap *atom_insert_new_ecoSysInMap( const IntVec2 &_sectionMPos );
bool atom_find_from_ecoSysesInMap( sectionKey_t _sectionKey );
EcoSysInMap *atom_get_ecoSysInMapPtr( sectionKey_t _sectionkey );



}//---------------------- namespace: esrc -------------------------//
#endif

