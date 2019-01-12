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
class AltiRange{
public:
    u8  low  {0}; //- 不应高过 jumpLimit
    u8  high {0}; //- 要比 low 值大

    static u8 jumpLimit; //- 此值仅仅用来限制 low

private:
};


//-- static --
inline u8 AltiRange::jumpLimit = 45;





#endif 

