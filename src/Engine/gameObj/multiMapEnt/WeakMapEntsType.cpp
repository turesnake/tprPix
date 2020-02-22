/*
 * ====================== WeakMapEntsType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "WeakMapEntsType.h"


//--------------- Libs ------------------//
#include "magic_enum.hpp"

//--------------- Engine ------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;



std::string weakMapEntsType_2_str( WeakMapEntsType t_ )noexcept{
    return std::string{ magic_enum::enum_name( t_ ) };
}



WeakMapEntsType str_2_weakMapEntsType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<WeakMapEntsType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find WeakMapEntsType: " << str_ << endl;
        tprAssert(0);
        return WeakMapEntsType::Empty; // never reach
    }
}









