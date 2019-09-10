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
};




inline std::string MapSurfaceLowSpec_2_str( const MapSurfaceLowSpec &spec_ )noexcept{
    switch (spec_){
        case MapSurfaceLowSpec::Test:  return "test";
        default:
            tprAssert(0);
            return ""; // never
    }
}




#endif 

