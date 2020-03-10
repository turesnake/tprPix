/*
 * ===================== GameObjType.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GameObjType.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

GameObjMoveState str_2_gameObjMoveState( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjMoveState>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find GameObjMoveState: {}", name_ );
        tprAssert(0);
        return GameObjMoveState::AbsFixed; // never reach
    }
}


GameObjState str_2_gameObjState( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjState>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find GameObjState: {}", name_ );
        tprAssert(0);
        return GameObjState::Sleep; // never reach
    }
}


GameObjFamily str_2_gameObjFamily( const std::string &name_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GameObjFamily>(name_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find GameObjFamily: {}", name_ );
        tprAssert(0);
        return GameObjFamily::Major; // never reach
    }
}



