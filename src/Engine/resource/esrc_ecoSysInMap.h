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
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "EcoSysInMap.h"
#include "IntVec.h"
#include "sectionKey.h"
#include "EcoSysInMap_ReadOnly.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


void atom_try_to_inert_and_init_a_ecoSysInMap( sectionKey_t _ecoSysInMapKey );

//-- 更加精细的 元素数据 只读访问 接口 [值传递] --
const std::pair<occupyWeight_t, EcoSysInMap_ReadOnly> atom_get_ecoSysInMap_readOnly( sectionKey_t _sectionkey );
const std::vector<RGBA> *atom_get_ecoSysInMap_landColorsPtr( sectionKey_t _sectionkey );

const goSpecId_t atom_ecoSysInMap_apply_a_rand_goSpecId( sectionKey_t _sectionkey, size_t _densityIdx, float _randV );
const float atom_ecoSysInMap_get_applyPercent( sectionKey_t _sectionkey, const Density &_density );



}//---------------------- namespace: esrc -------------------------//
#endif

