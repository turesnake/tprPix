/*
 * ===================== RenderLayerType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "RenderLayerType.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

//--------------- Engine ------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;



std::string renderLayerType_2_str( RenderLayerType dir_ )noexcept{
    return std::string{ magic_enum::enum_name( dir_ ) };
}


RenderLayerType str_2_renderLayerType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<RenderLayerType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find RenderLayerType: " << str_ << endl;
        tprAssert(0);
        return RenderLayerType::MajorGoes; // never reach
    }
}



