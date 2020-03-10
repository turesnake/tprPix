/*
 * ======================= ShaderType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ShaderType.h"


//--------------- Libs ------------------//
#include "magic_enum.hpp"



std::string shaderType_2_str( ShaderType type_ )noexcept{
    return std::string{ magic_enum::enum_name( type_ ) };
}


ShaderType str_2_shaderType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<ShaderType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find ShaderType: {}", str_ );
        tprAssert(0);
        return ShaderType::UnifiedColor; // never reach
    }
}





