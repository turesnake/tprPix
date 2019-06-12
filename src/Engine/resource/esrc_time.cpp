/*
 * ======================= esrc_time.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_time.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace time_inn {//-------- namespace: time_inn  --------------//

    TimeBase   timer {}; //-- 全局时间
    TimeCircle logicTimeCircle { &timer, 5 }; //- 逻辑时间循环 实例（5帧1周期）

}//------------- namespace: time_inn end --------------//

TimeBase &get_timer(){
    return time_inn::timer;
}


TimeCircle &get_logicTimeCircle(){
    return time_inn::logicTimeCircle;
}




}//---------------------- namespace: esrc -------------------------//


