/*
 * ======================= ShaderType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ShaderType.h"


//--------------- Libs ------------------//
#include "magic_enum.hpp"

//--------------- Engine ------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;



std::string shaderType_2_str( ShaderType type_ )noexcept{
    return std::string{ magic_enum::enum_name( type_ ) };
}


ShaderType str_2_shaderType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<ShaderType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find ShaderType: " << str_ << endl;
        tprAssert(0);
        return ShaderType::UnifiedColor; // never reach
    }
}





