/*
 * ===================== GoAltiRange.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GoAltiRange.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"


GoAltiRangeLabel str_2_goAltiRangeLabel( const std::string &str_ )noexcept{

    if( str_ == "" ){  
        return GoAltiRangeLabel::Default;
    }
    auto labelOP = magic_enum::enum_cast<GoAltiRangeLabel>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find GoAltiRangeLabel: {}", str_ );
        tprAssert(0);
        return GoAltiRangeLabel::Default; // never reach
    }
}


