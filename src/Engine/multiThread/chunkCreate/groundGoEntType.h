/*
 * ==================== groundGoEntType.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GROUND_GO_ENT_TYPE_H
#define TPR_GROUND_GO_ENT_TYPE_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"

enum class GroundGoEntType{
    MapEnt,    
    HalfField,
    Field,       // 相邻异色mapent，所以沿用 多边形 mesh
    SimpleField, // 相邻mapents 皆为同色，本身被简化为一个 正方形（减少计算
};

/*
inline GroundGoEntType str_2_groundGoEntType(const std::string &str_)noexcept{
    if(       str_ == "MapEnt" ){       return GroundGoEntType::MapEnt;
    }else if( str_ == "HalfField" ){    return GroundGoEntType::HalfField;
    }else if( str_ == "Field" ){        return GroundGoEntType::Field;
    }else if( str_ == "SimpleField" ){  return GroundGoEntType::SimpleField;
    }else{
        tprAssert(0);
        return GroundGoEntType::MapEnt; // never reach
    }
}
*/
/*
inline std::string groundGoEntType_2_str( GroundGoEntType type_ )noexcept{
    switch (type_){
        case GroundGoEntType::MapEnt:       return "MapEnt";
        case GroundGoEntType::HalfField:    return "HalfField";
        case GroundGoEntType::Field:        return "Field";
        case GroundGoEntType::SimpleField:  return "SimpleField";
        default:
            tprAssert(0);
            return "MapEnt"; // never reach
    }
}
*/


#endif 

