/*
 * ===================== GameObjType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GameObjType.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

//------------------- Engine --------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;



GameObjMoveState str_2_gameObjMoveState( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjMoveState>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find GameObjMoveState: " << name_ << endl;
        tprAssert(0);
        return GameObjMoveState::AbsFixed; // never reach
    }
}


GameObjState str_2_gameObjState( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjState>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find GameObjState: " << name_ << endl;
        tprAssert(0);
        return GameObjState::Sleep; // never reach
    }
}


GameObjFamily str_2_gameObjFamily( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjFamily>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find GameObjFamily: " << name_ << endl;
        tprAssert(0);
        return GameObjFamily::Major; // never reach
    }
}



