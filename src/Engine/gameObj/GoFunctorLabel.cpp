/*
 * ==================== GoFunctorLabel.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GoFunctorLabel.h"


//--------------- Libs ------------------//
#include "magic_enum.hpp"


// 手动设置 上下界
// 否则会按 [-128,128] 的默认范围。屏蔽掉额外的值
namespace magic_enum {
template <>
  struct customize::enum_range<GoFunctorLabel> {
    static constexpr int min = 0;
    static constexpr int max = 1000;
};
}


GoFunctorLabel str_2_goFunctorLabel( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<GoFunctorLabel>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find GoFunctorLabel: {}", str_ );
        tprAssert(0);
        return GoFunctorLabel::Nil; // never reach
    }
}


std::string goFunctorLabel_2_str( GoFunctorLabel l_ )noexcept{
    return std::string{ magic_enum::enum_name( l_ ) };
}











