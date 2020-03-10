/*
 * ====================== SignInMapEnts_Square_Type.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "SignInMapEnts_Square_Type.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"



std::string signInMapEnts_square_type_2_str( SignInMapEnts_Square_Type t_ )noexcept{
    return std::string{ magic_enum::enum_name( t_ ) };
}



SignInMapEnts_Square_Type str_2_signInMapEnts_square_type( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<SignInMapEnts_Square_Type>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find SignInMapEnts_Square_Type: {}", str_ );
        tprAssert(0);
        return SignInMapEnts_Square_Type::T_1m1; // never reach
    }
}



