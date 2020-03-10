/*
 * =================== AnimActionEName.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "AnimActionEName.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"


AnimActionEName str_2_animActionEName( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<AnimActionEName>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find AnimAction::AnimActionEName: {}", str_  );
        tprAssert(0);
        return AnimActionEName::Idle; // never reach
    }
}


std::string animActionEName_2_str( AnimActionEName a_ )noexcept{
    return std::string{ magic_enum::enum_name( a_ ) };
}


