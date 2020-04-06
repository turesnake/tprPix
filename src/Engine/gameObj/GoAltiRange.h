/*
 * ========================= GoAltiRange.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    go 高度区间, 不是 地图海拔，要和 Altitude 区分开来 
 * ----------------------------
 */
#ifndef TPR_ALTI_RANGE_H
#define TPR_ALTI_RANGE_H

//------------------- CPP --------------------//
#include <string>
#include <cstdint> // uint8_t


//------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprMath.h"


// go 的高度区间
class GoAltiRange{
public:
    //---- constructor -----//
    GoAltiRange() = default;
    GoAltiRange( double low_, double high_ ):
        low(low_),
        high(high_)
        { tprAssert(low <= high); }

    inline void clear_all()noexcept{
        low = 0.0;
        high = 0.0;
    }

    inline void set( double low_, double high_ )noexcept{
        tprAssert( low_ <= high_ );
        low  = low_;
        high = high_;
    }

    inline bool is_collide( const GoAltiRange& a_ )const noexcept{
        // 某个 碰撞体，高度间距几乎为0，直接判断不会发生碰撞
        if( is_closeEnough<double>(this->low, this->high, 0.01) || 
            is_closeEnough<double>(a_.low, a_.high, 0.01) ){
            return false;
        }
        if( is_closeEnough<double>( low, a_.low, 0.01 ) ){
            return true;
        }else if( low < a_.low ){
            return ((high>a_.low) ? true : false);
        }else{
            return ((a_.high>low) ? true : false);
        }
    }

    //======== vals ========//
    double  low  {0.0}; //- low <= high 其实相等是没有意义的，暂时保留
    double  high {0.0};
};


/* ===========================================================
 *                 operator +, -
 * -----------------------------------------------------------
 */
inline GoAltiRange operator + ( const GoAltiRange &a_, const GoAltiRange &b_ )noexcept{
    return  GoAltiRange{  a_.low + b_.low,
                          a_.high + b_.high };
}
inline GoAltiRange operator + ( const GoAltiRange &a_, double addAlti_ )noexcept{
    return  GoAltiRange{a_.low  + addAlti_,
                        a_.high + addAlti_ };
}

/* ===========================================================
 *                 is_GoAltiRange_collide
 * -----------------------------------------------------------
 */
inline bool is_GoAltiRange_collide( const GoAltiRange& a_, const GoAltiRange& b_ )noexcept{
    // 某个 碰撞体，高度间距几乎为0，直接判断不会发生碰撞
    if( is_closeEnough<double>(a_.low, a_.high, 0.01) || 
        is_closeEnough<double>(b_.low, b_.high, 0.01) ){
        return false;
    }
    if( is_closeEnough<double>( a_.low, b_.low, 0.01 ) ){
        return true;
    }else if( a_.low < b_.low ){
        return ((a_.high>b_.low) ? true : false);
    }else{
        return ((b_.high>a_.low) ? true : false);
    }
}


// 一个 go数据中，可能有数个 不同的 GoAltiRange 数据
// 比如，大树小树 的高度 
enum class GoAltiRangeLabel{
    Default=0,  // 只有一种时，推荐使用此值
                // json 文件中，可以直接用 "" 表达
    //---
    Big,
    Mid,
    Sml,

};

GoAltiRangeLabel str_2_goAltiRangeLabel( const std::string &str_ )noexcept;




#endif 

