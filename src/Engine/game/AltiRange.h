/*
 * ========================= AltiRange.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    高度区间 
 * ----------------------------
 */
#ifndef _TPR_ALTI_RANGE_H_
#define _TPR_ALTI_RANGE_H_

//------------------- Libs --------------------//
#include "tprDataType.h"

//-- 暂不关心 硬盘存储 -- 
//  0 <= val < 45:  major   -- (u8) [0, 45)
//  val ==     -1:  item    -- (u8) 255
//  val ==     -2:  surface -- (u8) 254 
class AltiRange{
public:
    //--- vals ---//
    char  low  {0}; //- 不应大于 jumpLimit
    char  high {0}; //- 要比 low 值大

    //---- static ----//
    static char jumpLimit; //- 此值仅仅用来限制 low
    static u8  diskAlti_item;    //- 在 animFrameSet 图片文件中，代表 item 的 高度值
    static u8  diskAlti_surface; //- 在 animFrameSet 图片文件中，代表 surface 的 高度值

    //---- funcs -----//
    inline void clear_all(){
        low = 0;
        high = 0;
    }
    inline void set( char _low, char _high ){
        low  = _low;
        high = _high;
    }
};

//-- static --
inline char AltiRange::jumpLimit = 45;
inline u8 AltiRange::diskAlti_item    = 255;
inline u8 AltiRange::diskAlti_surface = 254;



//-- item / surface --//
inline const AltiRange altiRange_item    {  (char)AltiRange::diskAlti_item, 
                                            (char)AltiRange::diskAlti_item };
inline const AltiRange altiRange_surface {  (char)AltiRange::diskAlti_surface, 
                                            (char)AltiRange::diskAlti_surface };


/* ===========================================================
 *                 is_AltiRange_collide
 * -----------------------------------------------------------
 */
inline bool is_AltiRange_collide( const AltiRange& _a, const AltiRange& _b ){

    bool rbool;

    if( _a.low == _b.low ){
        return true;
    }else if( _a.low < _b.low ){
        (_a.high>_b.low) ? rbool = true : rbool = false;
        return rbool;
    }else{
        (_b.high>_a.low) ? rbool = true : rbool = false;
        return rbool;
    }
}


#endif 

