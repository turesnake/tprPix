/*
 * ====================== FloorGoType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_FLOOR_GO_TYPE_H
#define TPR_GO_FLOOR_GO_TYPE_H

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




// floorGo 的渲染层次，本值影响 floorGo.goMeshZOff 值
// 在 蓝图 FD.png 数据中 
// ---
// 这样，我们就能实现：
// 地面拥有最基础的 nature 层 floorgo：L_0
// 在此之上，可以为房间内铺设地板：L_1
// 在地板上，可以铺设地毯：L_2
enum class FloorGoLayer{
    L_0, // goMeshZOff: (0.0, 0.1) (BOTTOM). 普通 fgo / nature fgo，大部分 road-fgo，都属于此， 水下
    L_1, // goMeshZOff: (0.1, 0.2) 地板
    L_2, // goMeshZOff: (0.2, 0.3) 地毯
    L_3, // goMeshZOff: (0.3, 0.4)
    L_4, // goMeshZOff: (0.4, 0.5) (TOP).
};



inline double floorGoLayer_2_goMesh_baseZOff( FloorGoLayer layer_ )noexcept{
    switch (layer_){
        case FloorGoLayer::L_0: return 0.0;
        case FloorGoLayer::L_1: return 0.1;
        case FloorGoLayer::L_2: return 0.2;
        case FloorGoLayer::L_3: return 0.3;
        case FloorGoLayer::L_4: return 0.4;
        default:
            tprAssert(0);
            return 0.0; // never reach
    }
}






#endif 

