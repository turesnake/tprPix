/*
 * ===================== NineDirection.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_NINE_DIRECTION_H
#define TPR_NINE_DIRECTION_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "IntVec.h"

enum class NineDirection{
    Mid         = 0, //- same as No_Direction
    Left        = 1,
    LeftTop     = 2,
    Top         = 3,
    RightTop    = 4,
    Right       = 5,
    RightBottom = 6,
    Bottom      = 7,
    LeftBottom  = 8
};
extern size_t nineDirectionSize;


inline NineDirection intVec2_2_nineDirection( IntVec2 v_ ) noexcept {
    if( v_.y < 0 ){
        if( v_.x<0 ){          return NineDirection::LeftBottom;
        }else if( v_.x==0 ){   return NineDirection::Bottom;
        }else{                  return NineDirection::RightBottom;
        }
    }else if( v_.y == 0 ){
        if( v_.x<0 ){         return NineDirection::Left;
        }else if( v_.x==0 ){  return NineDirection::Mid;
        }else{                 return NineDirection::Right;
        }
    }else{
        if( v_.x<0 ){         return NineDirection::LeftTop;
        }else if( v_.x==0 ){  return NineDirection::Top;
        }else{                 return NineDirection::RightTop;
        }
    }
}

inline NineDirection idx_2_nineDirection( size_t idx_ )noexcept{
    switch (idx_){
        case 0: return NineDirection::Mid;
        case 1: return NineDirection::Left;
        case 2: return NineDirection::LeftTop;
        case 3: return NineDirection::Top;
        case 4: return NineDirection::RightTop;
        case 5: return NineDirection::Right;
        case 6: return NineDirection::RightBottom;
        case 7: return NineDirection::Bottom;
        case 8: return NineDirection::LeftBottom;
        default:
            tprAssert(0);
            return NineDirection::Mid; // never reach
    }
}

/*
inline NineDirection apply_a_random_direction( double randV_ )noexcept{
        tprAssert( randV_ >= 0.0 );
    size_t idx = cast_2_size_t(randV_ * 13.7 + 71.9) % nineDirectionSize;
    return idx_2_nineDirection(idx);
}
*/


//-- 不包含 Mid --
// param: randUVal_ [0, 9999]
inline NineDirection apply_a_random_direction_without_mid( size_t randUVal_ )noexcept{
        
    size_t idx = (randUVal_ + 590179) % (nineDirectionSize-1);
    idx++; // 不含 0
    return idx_2_nineDirection(idx);
}


//---- only used for debug ----//
inline std::string nineDirection_2_str( NineDirection dir_ )noexcept{
    switch (dir_){
        case NineDirection::Mid:         return "Mid";
        case NineDirection::Left:        return "Left";
        case NineDirection::LeftTop:     return "LeftTop";
        case NineDirection::Top:         return "Top";
        case NineDirection::RightTop:    return "RightTop";
        case NineDirection::Right:       return "Right";
        case NineDirection::RightBottom: return "RightBottom";
        case NineDirection::Bottom:      return "Bottom";
        case NineDirection::LeftBottom:  return "LeftBottom";
        default:
            tprAssert(0);
            return "error"; // never reach
    }
}


#endif 

