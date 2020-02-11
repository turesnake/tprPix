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


// 手动设置 上下界
// 否则会按 [-128,128] 的默认范围。屏蔽掉额外的值
namespace magic_enum {
template <>
  struct enum_range<AnimLabel> {
    static constexpr int min = 0;
    static constexpr int max = 1000;
};
}


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


