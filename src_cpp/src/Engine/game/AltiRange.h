/*
 * ========================= AltiRange.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    go 高度区间, 不是 地图海拔，要和 Altitude 区分开来 
 * ----------------------------
 */
#ifndef TPR_ALTI_RANGE_H_
#define TPR_ALTI_RANGE_H_

//------------------- Libs --------------------//
#include "tprDataType.h"

//------------------- Engine --------------------//
#include "tprAssert.h"

//-- 暂不关心 硬盘存储 -- 
//  0 <= val <= 45:  major   -- (u8_t) [0, 45]
//  val ==     -1:  item    -- (u8_t) 254
//  val ==     -2:  surface -- (u8_t) 253 
class AltiRange{
public:
    //---- constructor -----//
    AltiRange() = default;
    AltiRange( char low_, char high_ ):
        low(low_),
        high(high_)
        { tprAssert( (low<=high) && (low<=jumpLimit) ); }

    inline void clear_all(){
        low = 0;
        high = 0;
    }
    inline void set( char low_, char high_ ){
        tprAssert( (low_<=high_) && (low_<=jumpLimit) );
        low  = low_;
        high = high_;
    }

    // 在一个 给定 AltiRange值的基础上，类加上一个 addAlti_.
    // 新的值 设置为本实例的值。
    //-- 常用于 碰撞检测 --
    inline void set_by_addAlti( const AltiRange &a_, float addAlti_ ){
        tprAssert( (addAlti_<static_cast<float>(jumpLimit)) && (a_.low+static_cast<u8_t>(addAlti_))<=jumpLimit );
        low  =  a_.low  + static_cast<u8_t>(addAlti_);
        high =  a_.high + static_cast<u8_t>(addAlti_);
    }

    inline bool is_collide( const AltiRange& a_ ){
        bool rbool;
        if( low == a_.low ){
            return true;
        }else if( low < a_.low ){
            (high>a_.low) ? rbool=true : rbool=false;
            return rbool;
        }else{
            (a_.high>low) ? rbool=true : rbool=false;
            return rbool;
        }
    }

    //======== static ========//
    static char  jumpLimit;       //- tprAssert( low <= jumpLimit );
    static u8_t  diskAlti_item;    //- 在 animFrameSet 图片文件中，代表 item 的 高度值
    static u8_t  diskAlti_surface; //- 在 animFrameSet 图片文件中，代表 surface 的 高度值

    //======== vals ========//
    char  low  {0}; //- low <= jumpLimit
    char  high {0}; //- high >= low (其实相等是没什么意义的)
};

//-- static --
inline char AltiRange::jumpLimit = 45;
inline u8_t AltiRange::diskAlti_item    = 254;
inline u8_t AltiRange::diskAlti_surface = 253;
            //- 原本是 255/254; 但这样就会与 AnimFrameSet 中的 rootAnchor 信息冲突
            //- 故暂时下调1，改为 254/253
            //  在未来。随着 碰撞系统的不断完善。 surface_go/item_go 这组设计很可能会被取代...


//-- item / surface --//
inline const AltiRange altiRange_item    {  static_cast<char>(AltiRange::diskAlti_item), 
                                            static_cast<char>(AltiRange::diskAlti_item) };
inline const AltiRange altiRange_surface {  static_cast<char>(AltiRange::diskAlti_surface), 
                                            static_cast<char>(AltiRange::diskAlti_surface) };


/* ===========================================================
 *                 operator +, -
 * -----------------------------------------------------------
 */
inline AltiRange operator + ( const AltiRange &a_, const AltiRange &b_ ){
    tprAssert( (a_.low+b_.low)<=AltiRange::jumpLimit );
    return  AltiRange{  static_cast<char>(a_.low+b_.low),
                        static_cast<char>(a_.high+b_.high) };
                            //-- 此处有个问题。 两个 char 的 加法 会被自动提升为 int间的加法...
}

inline AltiRange operator + ( const AltiRange &a_, float addAlti_ ){
    tprAssert( (addAlti_<static_cast<float>(AltiRange::jumpLimit)) && (a_.low+static_cast<u8_t>(addAlti_))<=AltiRange::jumpLimit );
    return  AltiRange{  static_cast<char>(a_.low  + static_cast<char>(addAlti_)),
                        static_cast<char>(a_.high + static_cast<char>(addAlti_)) };
                            //-- 此处有个问题。 两个 char 的 加法 会被自动提升为 int间的加法...
}


/* ===========================================================
 *                 is_AltiRange_collide
 * -----------------------------------------------------------
 */
inline bool is_AltiRange_collide( const AltiRange& a_, const AltiRange& b_ ){
    if( a_.low == b_.low ){
        return true;
    }else if( a_.low < b_.low ){
        return ((a_.high>b_.low) ? true : false);
    }else{
        return ((b_.high>a_.low) ? true : false);
    }
}


#endif 

