/*
 * =================== MapSurfaceRandLvl.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_SURFACE_RAND_LVL_H
#define TPR_MAP_SURFACE_RAND_LVL_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"

enum class MapSurfaceRandLvl{
    Nil, //- none
    Sml,
    Mid,
    Big
};

inline std::string MapSurfaceRandLvl_2_str( const MapSurfaceRandLvl &lvl_ )noexcept{
    switch (lvl_){
        case MapSurfaceRandLvl::Sml: return "sml";
        case MapSurfaceRandLvl::Mid: return "mid";
        case MapSurfaceRandLvl::Big: return "big";
        default:
            tprAssert(0);
            return ""; // never reach
    }
}






#endif 

