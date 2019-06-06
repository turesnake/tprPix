/*
 * ======================= time_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_time.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace {//-------- namespace: --------------//

    TimeBase   timer {}; //-- 全局时间
    TimeCircle logicTimeCircle { &timer, 5 }; //- 逻辑时间循环 实例（5帧1周期）

}//------------- namespace: end --------------//

TimeBase &get_timer(){
    return timer;
}


TimeCircle &get_logicTimeCircle(){
    return logicTimeCircle;
}




}//---------------------- namespace: esrc -------------------------//


