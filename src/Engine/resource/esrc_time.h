/*
 * ========================= esrc_time.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_TIME_H_
#define TPR_ESRC_TIME_H_

//-------------------- Engine --------------------//
#include "TimeBase.h" 
#include "TimeCircle.h" 

namespace esrc {//------------------ namespace: esrc -------------------------//


TimeBase &get_timer();
TimeCircle &get_logicTimeCircle();


}//---------------------- namespace: esrc -------------------------//
#endif

