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
#include <set>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-------------------- Engine --------------------//
#include "MapAltitude.h"
#include "Density.h"
#include "MapSurfaceRandEnt.h"
#include "AnimLabel.h"

#include "Job_ChunkCreate.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/mapSurfaces/MapSurfaceSpec.h"


enum class ParamBinaryType{
    Nil,
    MapSurface,
    Field, //- 临时简陋版
    GroundGo
};


//-- 简陋的临时版 ，传递 field 相关的 常规随机数 --
struct DyParams_Field{
    double       fieldUWeight          {};
	MapAltitude  fieldNodeMapEntAlti  {};
	Density      fieldDensity         {};
    std::vector<AnimLabel> animLabels {}; //- maybe empty
};


struct DyParams_MapSurface{
    MapSurfaceLowSpec       spec     {}; // "mapSurfaceLow_test"
    mapSurface::RandEntLvl  lvl      {}; // "Field_1f1"
    double                  randVal  {}; // field.weight
};


struct DyParams_GroundGo{
    double      fieldUWeight     {};
    const Job_Field  *job_fieldPtr     {nullptr};
};





//-- 参数二进制包，目前仅用于 go create 中的 动态参数传递
//   一个很简陋的方案
class ParamBinary{
public:
    ParamBinary() = default;
    inline ParamBinaryType get_type() const noexcept{ return this->type; }

    template< typename T >
    inline T *init_binary( ParamBinaryType type_ )noexcept{
        this->type = type_;
        switch (type_){
        case ParamBinaryType::Field: tprAssert( sizeof(T) == sizeof(DyParams_Field) ); break;
        case ParamBinaryType::MapSurface: tprAssert( sizeof(T) == sizeof(DyParams_MapSurface) ); break;
        case ParamBinaryType::GroundGo: tprAssert( sizeof(T) == sizeof(DyParams_GroundGo) ); break;
        case ParamBinaryType::Nil:    
        default:
            tprAssert(0);
            break;
        }
        this->binary.resize( sizeof(T), 0 );
        return reinterpret_cast<T*>( &(this->binary.at(0)) );
    }

    template< typename T >
    inline const T*get_binaryPtr()const noexcept{
        tprAssert( sizeof(T) == this->binary.size() );
        return reinterpret_cast<const T*>( &(this->binary.at(0)) );
    }

private:
    ParamBinaryType    type {ParamBinaryType::Nil};
    std::vector<u8_t>  binary {}; //- 数据本体
};


//- 空参数包 -
extern ParamBinary emptyParamBinary;

#endif 

