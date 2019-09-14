/*
 * ===================== MapSurfaceSpec.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_SURFACE_SPEC_H
#define TPR_MAP_SURFACE_SPEC_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"


enum class MapSurfaceLowSpec{
    Nil,
    Test, // 简陋的线框，表示范围
    WhiteRock,
};




inline std::string MapSurfaceLowSpec_2_str( MapSurfaceLowSpec spec_ )noexcept{
    switch (spec_){
        case MapSurfaceLowSpec::Test:       return "mapSurfaceLow_test";
        case MapSurfaceLowSpec::WhiteRock:  return "mapSurfaceLow_whiteRock";
        default:
            tprAssert(0);
            return ""; // never reach
    }
}




#endif 

