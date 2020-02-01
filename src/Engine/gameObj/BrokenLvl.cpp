/*
 * ====================== BrokenLvl.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "BrokenLvl.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

//------------------- Engine --------------------//
#include "tprAssert.h"


#include "tprDebug.h"



std::string brokenLvl_2_str( BrokenLvl bl_ )noexcept{
    return std::string{ magic_enum::enum_name( bl_ ) };
}



