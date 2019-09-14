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
#include "AnimLabel.h"

enum class MapSurfaceRandLvl{
    Nil, //- none
    Sml,
    Mid,
    Big
};

inline std::string MapSurfaceRandLvl_2_str( MapSurfaceRandLvl lvl_ )noexcept{
    switch (lvl_){
        case MapSurfaceRandLvl::Sml: return "sml";
        case MapSurfaceRandLvl::Mid: return "mid";
        case MapSurfaceRandLvl::Big: return "big";
        default:
            tprAssert(0);
            return ""; // never reach
    }
}

inline AnimLabel MapSurfaceRandLvl_2_AnimLabel( MapSurfaceRandLvl lvl_  )noexcept{
    switch (lvl_){
        case MapSurfaceRandLvl::Sml: return AnimLabel::Sml;
        case MapSurfaceRandLvl::Mid: return AnimLabel::Mid;
        case MapSurfaceRandLvl::Big: return AnimLabel::Big;
        default:
            tprAssert(0);
            return AnimLabel::Default; // never reach
    }
}


#endif 

