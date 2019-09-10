/*
 * ===================== ParamBinary.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "ParamBinary.h"

//------------------- Engine --------------------//
#include "tprAssert.h"


/* ===========================================================
 *                    init_binary
 * -----------------------------------------------------------
 */
u8_t *ParamBinary::init_binary( const ParamBinaryType &type_ ){


    //goRef_.resize_pvtBinary( sizeof(MapSurfaceLower_PvtBinary) );
    //auto *pvtBp = reinterpret_cast<MapSurfaceLower_PvtBinary*>(goRef_.get_pvtBinaryPtr());

    this->type = type_;

    switch (type_){
    case ParamBinaryType::Field:
        this->binary.resize( sizeof(DyParams_Field) );
        break;


    case ParamBinaryType::MapSurface:
        this->binary.resize( sizeof(DyParams_MapSurface) );
        break;

    case ParamBinaryType::Nil:    
    default:
        tprAssert(0);
        break;
    }

    return &(this->binary.at(0));//- 半通用指针
}




