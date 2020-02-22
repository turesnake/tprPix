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

//--------------- Engine ------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;



std::string nineDirection_2_str( NineDirection dir_ )noexcept{
    return std::string{ magic_enum::enum_name( dir_ ) };
}


NineDirection str_2_nineDirection( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<NineDirection>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find NineDirection: " << str_ << endl;
        tprAssert(0);
        return NineDirection::Center; // never reach
    }
}



