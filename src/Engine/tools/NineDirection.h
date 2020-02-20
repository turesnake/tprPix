/*
 * ===================== NineDirection.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_NINE_DIRECTION_H
#define TPR_NINE_DIRECTION_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "IntVec.h"

enum class NineDirection{
    Center      = 0, //- same as No_Direction
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
        }else if( v_.x==0 ){  return NineDirection::Center;
        }else{                 return NineDirection::Right;
        }
    }else{
        if( v_.x<0 ){         return NineDirection::LeftTop;
        }else if( v_.x==0 ){  return NineDirection::Top;
        }else{                 return NineDirection::RightTop;
        }
    }
}


inline NineDirection dpos_2_nineDirection( const glm::dvec2 &v_ ) noexcept {
    if( v_.y < 0.0 ){
        if( v_.x<0.0 ){          return NineDirection::LeftBottom;
        }else if( v_.x==0.0 ){   return NineDirection::Bottom;
        }else{                  return NineDirection::RightBottom;
        }
    }else if( v_.y == 0.0 ){
        if( v_.x<0.0 ){         return NineDirection::Left;
        }else if( v_.x==0.0 ){  return NineDirection::Center;
        }else{                 return NineDirection::Right;
        }
    }else{
        if( v_.x<0.0 ){         return NineDirection::LeftTop;
        }else if( v_.x==0.0 ){  return NineDirection::Top;
        }else{                 return NineDirection::RightTop;
        }
    }
}



inline NineDirection idx_2_nineDirection( size_t idx_ )noexcept{
    switch (idx_){
        case 0: return NineDirection::Center;
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
            return NineDirection::Center; // never reach
    }
}

//-- 不包含 Center --
// param: randUVal_ [0, 9999]
inline NineDirection apply_a_random_direction_without_mid( size_t randUVal_ )noexcept{
        
    size_t idx = (randUVal_ + 590179) % (nineDirectionSize-1);
    idx++; // 不含 0
    return idx_2_nineDirection(idx);
}

//---- only used for debug ----//
std::string nineDirection_2_str( NineDirection dir_ )noexcept;


NineDirection str_2_nineDirection( const std::string &str_ )noexcept;


inline IntVec2 nineDirection_2_mposOff( NineDirection dir_ )noexcept{
    switch (dir_){
        case NineDirection::Center:      return IntVec2{  0,  0 };
        case NineDirection::Left:        return IntVec2{ -1,  0 };
        case NineDirection::LeftTop:     return IntVec2{ -1,  1 };
        case NineDirection::Top:         return IntVec2{  0,  1 };
        case NineDirection::RightTop:    return IntVec2{  1,  1 };
        case NineDirection::Right:       return IntVec2{  1,  0 };
        case NineDirection::RightBottom: return IntVec2{  1, -1 };
        case NineDirection::Bottom:      return IntVec2{  0, -1 };
        case NineDirection::LeftBottom:  return IntVec2{ -1, -1 };
        default:
            tprAssert(0);
            return IntVec2{}; // never reach
    }
}

//-返回的 dvec2 仅仅表达一个 方向，并不是 单位向量
inline glm::dvec2 nineDirection_2_dVec2( NineDirection dir_ )noexcept{
    switch (dir_){
        case NineDirection::Center:      return glm::dvec2{  0.0,  0.0 };
        case NineDirection::Left:        return glm::dvec2{ -1.0,  0.0 };
        case NineDirection::LeftTop:     return glm::dvec2{ -1.0,  1.0 };
        case NineDirection::Top:         return glm::dvec2{  0.0,  1.0 };
        case NineDirection::RightTop:    return glm::dvec2{  1.0,  1.0 };
        case NineDirection::Right:       return glm::dvec2{  1.0,  0.0 };
        case NineDirection::RightBottom: return glm::dvec2{  1.0, -1.0 };
        case NineDirection::Bottom:      return glm::dvec2{  0.0, -1.0 };
        case NineDirection::LeftBottom:  return glm::dvec2{ -1.0, -1.0 };
        default:
            tprAssert(0);
            return glm::dvec2{  0.0, 0.0 }; // never reach
    }
}

//- 是否为 斜向 方向
inline bool is_diagonalDir( NineDirection dir_ )noexcept{
    switch (dir_){
        case NineDirection::LeftTop:       
        case NineDirection::RightTop:    
        case NineDirection::RightBottom: 
        case NineDirection::LeftBottom: 
            return true;
        case NineDirection::Center:         
        case NineDirection::Left:        
        case NineDirection::Right: 
        case NineDirection::Top:  
        case NineDirection::Bottom: 
            return false;
        default:
            tprAssert(0);
            return false; // never reach
    }
}




#endif 

