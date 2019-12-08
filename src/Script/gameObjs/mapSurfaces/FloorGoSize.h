/*
 * ====================== FloorGoSize.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_FLOOR_GO_SIZE_H
#define TPR_GO_FLOOR_GO_SIZE_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


#include <string>

#include "config.h"
#include "tprAssert.h"


enum class FloorGoSize{
    MapEnt_2m2,
    MapEnt_3m3,
    MapEnt_4m4,
};


inline FloorGoSize str_2_FloorGoSize( const std::string &str_ )noexcept{
    if(       str_ == "MapEnt_2m2" ){   return FloorGoSize::MapEnt_2m2;
    }else if( str_ == "MapEnt_3m3" ){   return FloorGoSize::MapEnt_3m3;
    }else if( str_ == "MapEnt_4m4" ){   return FloorGoSize::MapEnt_4m4;
    }else{
        tprAssert(0);
        return FloorGoSize::MapEnt_2m2;
    }
}


// 计算某种 floorGo，从其 中点dpos，到 中点所在的 mpos 的 偏移值
inline glm::dvec2 calc_floorGo_mid_dposOff( FloorGoSize size_ )noexcept{

    double pixes_per_mapent = static_cast<double>(PIXES_PER_MAPENT);
    switch (size_){
        case FloorGoSize::MapEnt_2m2: return glm::dvec2{ 0.0, 0.0 };
        case FloorGoSize::MapEnt_3m3: return glm::dvec2{ 0.5*pixes_per_mapent, 0.5*pixes_per_mapent };
        case FloorGoSize::MapEnt_4m4: return glm::dvec2{ 0.0, 0.0 };
        default:
            tprAssert(0);
            return glm::dvec2{};
            break;
    }
}




#endif 

