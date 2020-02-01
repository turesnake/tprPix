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

#include "tprDebug.h" 



GameObjMoveState str_2_GameObjMoveState( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjMoveState>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find GameObjMoveState: " << name_ << endl;
        tprAssert(0);
        return GameObjMoveState::AbsFixed; // never reach
    }
}


GameObjState str_2_GameObjState( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjState>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find GameObjState: " << name_ << endl;
        tprAssert(0);
        return GameObjState::Sleep; // never reach
    }
}


GameObjFamily str_2_GameObjFamily( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjFamily>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find GameObjFamily: " << name_ << endl;
        tprAssert(0);
        return GameObjFamily::Major; // never reach
    }
}


