/*
 * ======================== AnimLabel.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimLabel.h"


//--------------- Libs ------------------//
#include "magic_enum.hpp"

//-------------------- Engine --------------------//
#include "tprAssert.h"

#include "tprDebug.h"


AnimLabel str_2_AnimLabel( const std::string &str_ )noexcept{

    if( str_ == "" ){  
        return AnimLabel::Default;
    }

    auto labelOP = magic_enum::enum_cast<AnimLabel>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find animLabel: " << str_ << endl;
        tprAssert(0);
        return AnimLabel::Default; // never reach
    }
}


