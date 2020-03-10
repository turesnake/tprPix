/*
 * ====================== BrokenLvl.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "BrokenLvl.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"


std::string brokenLvl_2_str( BrokenLvl bl_ )noexcept{
    return std::string{ magic_enum::enum_name( bl_ ) };
}


BrokenLvl str_2_brokenLvl( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<BrokenLvl>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find BrokenLvl: {}", str_ );
        tprAssert(0);
        return BrokenLvl::Lvl_0; // never reach
    }
}



