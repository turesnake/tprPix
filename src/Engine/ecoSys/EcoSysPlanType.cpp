/*
 * ====================== EcoSysPlanType.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "EcoSysPlanType.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

EcoSysPlanType str_2_ecoSysPlanType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<EcoSysPlanType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find EcoSysPlanType: {}", str_ );
        tprAssert(0);
        return EcoSysPlanType::BegIdx; // never reach
    }
}





