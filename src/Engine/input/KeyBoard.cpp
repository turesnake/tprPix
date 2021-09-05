/*
 * ========================= KeyBoard.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"


#include "KeyBoard.h" // After magic_enum.hpp


// 手动设置 上下界
// 否则会按 [-128,128] 的默认范围。屏蔽掉额外的值
namespace magic_enum {
template <>
  struct customize::enum_range<KeyBoard::Key> {
    static constexpr int min = -100;
    static constexpr int max = 1000;
};
}


std::string keyBoardKey_2_str( KeyBoard::Key kb_ )noexcept{
    return std::string{ magic_enum::enum_name( kb_ ) };
}


