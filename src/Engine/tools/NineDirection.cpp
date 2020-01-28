/*
 * ===================== NineDirection.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "NineDirection.h"

size_t nineDirectionSize {9};


//--------------- Libs ------------------//
#include "magic_enum.hpp"

#include "tprDebug.h"



std::string nineDirection_2_str( NineDirection dir_ )noexcept{
    return std::string{ magic_enum::enum_name( dir_ ) };
}



