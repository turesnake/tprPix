/*
 * ========================= esrc_time.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_TIME_H
#define TPR_ESRC_TIME_H

//-------------------- Engine --------------------//
#include "TimeBase.h" 
#include "TimeCircle.h" 
#include "WindClock.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


void init_time()noexcept;
TimeBase &get_timer()noexcept;
TimeCircle &get_logicTimeCircle()noexcept;
WindClock &get_windClock()noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif

