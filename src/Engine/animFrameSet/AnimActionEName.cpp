/*
 * =================== AnimActionEName.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimActionEName.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

//--------------- Engine ------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;


AnimActionEName str_2_animActionEName( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<AnimActionEName>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find AnimActionEName: " << str_ << endl;
        tprAssert(0);
        return AnimActionEName::Idle; // never reach
    }
}


std::string animActionEName_2_str( AnimActionEName a_ )noexcept{
    return std::string{ magic_enum::enum_name( a_ ) };
}


