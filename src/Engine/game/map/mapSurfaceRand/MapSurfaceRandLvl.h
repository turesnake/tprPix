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

namespace mapSurface {//------------------ namespace: mapSurface -------------------------//

enum class RandEntLvl{
    Nil, //- none
    Field_1f1,
    Field_2f2,
    Field_3f3,
};

inline RandEntLvl str_2_randEntLvl( const std::string &str_ )noexcept{
    if(         str_ == "Field_1f1" ){ return RandEntLvl::Field_1f1;
    }else if(   str_ == "Field_2f2" ){ return RandEntLvl::Field_2f2;
    }else if(   str_ == "Field_3f3" ){ return RandEntLvl::Field_3f3;
    }else if(   str_ == "Nil"       ){ 
        tprAssert(0); // tmp
        return RandEntLvl::Nil;
    }else{
        tprAssert(0);
        return RandEntLvl::Nil;
    }
}

inline AnimLabel mapSurfaceRandEntLvl_2_AnimLabel( RandEntLvl lvl_  )noexcept{
    switch (lvl_){
        case RandEntLvl::Field_1f1: return AnimLabel::Field_1f1;
        case RandEntLvl::Field_2f2: return AnimLabel::Field_2f2;
        case RandEntLvl::Field_3f3: return AnimLabel::Field_3f3;
        default:
            tprAssert(0);
            return AnimLabel::Default; // never reach
    }
}


enum class RandMeshLvl{
    Nil, //- none
    MapEnt_1m1,
    MapEnt_2m2,
    MapEnt_3m3,
    MapEnt_4m4
};


inline AnimLabel mapSurfaceRandMeshLvl_2_AnimLabel( RandMeshLvl lvl_  )noexcept{
    switch (lvl_){
        case RandMeshLvl::MapEnt_1m1: return AnimLabel::MapEnt_1m1;
        case RandMeshLvl::MapEnt_2m2: return AnimLabel::MapEnt_2m2;
        case RandMeshLvl::MapEnt_3m3: return AnimLabel::MapEnt_3m3;
        case RandMeshLvl::MapEnt_4m4: return AnimLabel::MapEnt_4m4;
        default:
            tprAssert(0);
            return AnimLabel::Default; // never reach
    }
}



}//---------------------- namespace: mapSurface -------------------------//
#endif 

