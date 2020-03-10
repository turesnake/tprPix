/*
 * ===================== FloorGoType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "FloorGoType.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"


FloorGoSize str_2_floorGoSize( const std::string &str_ )noexcept{
    auto labelOP = magic_enum::enum_cast<FloorGoSize>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find FloorGoSize: {}", str_ );
        tprAssert(0);
        return FloorGoSize::MapEnt_2m2; // never reach
    }
}




FloorGoLayer str_2_floorGoLayer( const std::string &str_ )noexcept{
    auto labelOP = magic_enum::enum_cast<FloorGoLayer>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find FloorGoLayer: {}", str_ );
        tprAssert(0);
        return FloorGoLayer::L_0; // never reach
    }
}








