/*
 * ========================= esrc_ecoSys.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_ECOSYS_H_
#define _TPR_ESRC_ECOSYS_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "EcoSys.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

inline std::unordered_map<ecoSysId_t, EcoSys> ecoSyses {};

void init_ecoSyses();

inline EcoSys *get_ecoSysPtr( ecoSysId_t _ecoId ){
        assert( esrc::ecoSyses.find(_ecoId) != esrc::ecoSyses.end() ); //- tmp
    return static_cast<EcoSys*>( &(esrc::ecoSyses.at(_ecoId)) );
}

//-- 一种type，一个子容器，容纳此type 的所有变种 --
inline std::vector<std::vector<ecoSysId_t>> ecoSysIds_in_type {};

ecoSysId_t apply_a_ecoSysId_by_type( EcoSysType _type, float _ecoSysInMapWeight );

//-- 仅记录所有 ecoId 值 --
inline std::vector<ecoSysId_t> ecoSysIds {};

ecoSysId_t apply_a_rand_ecoSysId( float _ecoSysInMapWeight );


}//---------------------- namespace: esrc -------------------------//
#endif

