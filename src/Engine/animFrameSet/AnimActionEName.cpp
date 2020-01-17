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


#include "tprDebug.h"


AnimActionEName str_2_AnimActionEName( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<AnimActionEName>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find AnimActionEName: " << str_ << endl;
        tprAssert(0);
        return AnimActionEName::Idle; // never reach
    }
}


