/*
 * ===================== NineDirection.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "NineDirection.h"

size_t nineDirectionSize {9};

//--------------- Libs ------------------//
#include "magic_enum.hpp"


std::string nineDirection_2_str( NineDirection dir_ )noexcept{
    return std::string{ magic_enum::enum_name( dir_ ) };
}


NineDirection str_2_nineDirection( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<NineDirection>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find NineDirection: {}", str_ );
        tprAssert(0);
        return NineDirection::Center; // never reach
    }
}



