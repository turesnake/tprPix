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

//------------------- Libs --------------------//
#include "tprDataType.h"

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
        low = 0;
        high = 0;
    }

    inline void set( double low_, double high_ )noexcept{
        tprAssert( low_ <= high_ );
        low  = low_;
        high = high_;
    }

    inline bool is_collide( const GoAltiRange& a_ )noexcept{
        bool rbool;
        if( is_closeEnough<double>( low, a_.low, 0.01 ) ){
            return true;
        }else if( low < a_.low ){
            (high>a_.low) ? rbool=true : rbool=false;
            return rbool;
        }else{
            (a_.high>low) ? rbool=true : rbool=false;
            return rbool;
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

    if( is_closeEnough<double>( a_.low, b_.low, 0.01 ) ){
        return true;
    }else if( a_.low < b_.low ){
        return ((a_.high>b_.low) ? true : false);
    }else{
        return ((b_.high>a_.low) ? true : false);
    }
}


#endif 

