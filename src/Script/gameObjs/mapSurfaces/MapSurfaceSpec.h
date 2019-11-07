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
    DForest, // 
    Forest,
};


inline MapSurfaceLowSpec str_2_MapSurfaceLowSpec( const std::string &str_ )noexcept{
    if( str_        == "Nil" ){         return MapSurfaceLowSpec::Nil;
    }else if( str_  == "Test" ){        return MapSurfaceLowSpec::Test;
    }else if( str_  == "DForest" ){     return MapSurfaceLowSpec::DForest;
    }else if( str_  == "Forest" ){      return MapSurfaceLowSpec::Forest;
    }else{
        tprAssert(0);
        return MapSurfaceLowSpec::Nil;
    }
}



inline std::string MapSurfaceLowSpec_2_str( MapSurfaceLowSpec spec_ )noexcept{
    switch (spec_){
        case MapSurfaceLowSpec::Test:       return "mapSurfaceLow_test";
        case MapSurfaceLowSpec::DForest:    return "mapSurfaceLow_dforest";
        case MapSurfaceLowSpec::Forest:    return "mapSurfaceLow_forest";
        default:
            tprAssert(0);
            return ""; // never reach
    }
}




#endif 

