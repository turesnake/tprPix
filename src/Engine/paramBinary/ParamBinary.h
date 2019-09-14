/*
 * ======================= ParamBinary.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_PARAM_BINARY_H
#define TPR_PARAM_BINARY_H

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-------------------- Engine --------------------//
#include "MapAltitude.h"
#include "Density.h"
#include "MapSurfaceRandSet.h"
#include "AnimLabel.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/mapSurfaces/MapSurfaceSpec.h"


enum class ParamBinaryType{
    Nil,
    MapSurface,
    Field, //- 临时简陋版
};



//-- 参数二进制包，目前仅用于 go create 中的 动态参数传递
//   一个很简陋的方案
class ParamBinary{
public:
    ParamBinary() = default;
    u8_t *init_binary( ParamBinaryType type_ );
    inline const u8_t *get_binaryPtr() const noexcept{ return &(this->binary.at(0)); }
    inline ParamBinaryType get_type() const noexcept{ return this->type; }
private:
    ParamBinaryType    type {ParamBinaryType::Nil};
    std::vector<u8_t>  binary {}; //- 数据本体
};


//- 空参数包 -
inline ParamBinary emptyParamBinary {};


//-- 简陋的临时版 ，传递 field 相关的 常规随机数 --
struct DyParams_Field{
    double       fieldWeight          {};
	MapAltitude  fieldNodeMapEntAlti  {};
	Density      fieldDensity         {};
    std::vector<AnimLabel> animLabels {}; //- maybe empty
};


struct DyParams_MapSurface{
    MapSurfaceLowSpec  spec     {}; // "mapSurfaceLow_test"
    MapSurfaceRandLvl  lvl      {}; // "sml"
    double             randVal  {}; // field.weight
};



template< typename T >
inline const T*cast_2_dyParamBinaryPtr( const ParamBinary &dyParams_ )noexcept{
    // 暂时无法检测 dyParams_ 类型 ...
    return reinterpret_cast<const T*>( dyParams_.get_binaryPtr() );
}




#endif 

