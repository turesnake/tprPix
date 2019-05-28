/*
 * ========================= esrc_time.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_TIME_H_
#define _TPR_ESRC_TIME_H_

//-------------------- Engine --------------------//
#include "TimeBase.h" 
#include "TimeCircle.h" 

namespace esrc {//------------------ namespace: esrc -------------------------//


inline TimeBase   timer {}; //-- 全局时间
inline TimeCircle logicTimeCircle { &timer, 5 }; //- 逻辑时间循环 实例（5帧1周期）


}//---------------------- namespace: esrc -------------------------//
#endif

