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
    AltiRange( char _low, char _high ):
        low(_low),
        high(_high)
        { tprAssert( (low<=high) && (low<=jumpLimit) ); }

    inline void clear_all(){
        low = 0;
        high = 0;
    }
    inline void set( char _low, char _high ){
        tprAssert( (_low<=_high) && (_low<=jumpLimit) );
        low  = _low;
        high = _high;
    }

    // 在一个 给定 AltiRange值的基础上，类加上一个 _addAlti.
    // 新的值 设置为本实例的值。
    //-- 常用于 碰撞检测 --
    inline void set_by_addAlti( const AltiRange &_a, float _addAlti ){
        tprAssert( (_addAlti<static_cast<float>(jumpLimit)) && (_a.low+static_cast<u8_t>(_addAlti))<=jumpLimit );
        low  =  _a.low  + static_cast<u8_t>(_addAlti);
        high =  _a.high + static_cast<u8_t>(_addAlti);
    }

    inline bool is_collide( const AltiRange& _a ){
        bool rbool;
        if( low == _a.low ){
            return true;
        }else if( low < _a.low ){
            (high>_a.low) ? rbool=true : rbool=false;
            return rbool;
        }else{
            (_a.high>low) ? rbool=true : rbool=false;
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
inline AltiRange operator + ( const AltiRange &_a, const AltiRange &_b ){
    tprAssert( (_a.low+_b.low)<=AltiRange::jumpLimit );
    return  AltiRange{  static_cast<char>(_a.low+_b.low),
                        static_cast<char>(_a.high+_b.high) };
                            //-- 此处有个问题。 两个 char 的 加法 会被自动提升为 int间的加法...
}

inline AltiRange operator + ( const AltiRange &_a, float _addAlti ){
    tprAssert( (_addAlti<static_cast<float>(AltiRange::jumpLimit)) && (_a.low+static_cast<u8_t>(_addAlti))<=AltiRange::jumpLimit );
    return  AltiRange{  static_cast<char>(_a.low  + static_cast<char>(_addAlti)),
                        static_cast<char>(_a.high + static_cast<char>(_addAlti)) };
                            //-- 此处有个问题。 两个 char 的 加法 会被自动提升为 int间的加法...
}


/* ===========================================================
 *                 is_AltiRange_collide
 * -----------------------------------------------------------
 */
inline bool is_AltiRange_collide( const AltiRange& _a, const AltiRange& _b ){
    if( _a.low == _b.low ){
        return true;
    }else if( _a.low < _b.low ){
        return ((_a.high>_b.low) ? true : false);
    }else{
        return ((_b.high>_a.low) ? true : false);
    }
}


#endif 

