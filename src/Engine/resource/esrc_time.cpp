/*
 * ======================= esrc_time.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_time.h"
#include "esrc_state.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace time_inn {//-------- namespace: time_inn  --------------//

    std::unique_ptr<TimeBase>       timerUPtr {nullptr};
    std::unique_ptr<TimeCircle>     logicTimeCircleUPtr {nullptr};
    std::unique_ptr<WindClock>      windClockUPtr {nullptr};

}//------------- namespace: time_inn end --------------//

void init_time()noexcept{
    time_inn::timerUPtr = std::make_unique<TimeBase>();
    time_inn::logicTimeCircleUPtr = std::make_unique<TimeCircle>( *(time_inn::timerUPtr.get()), 5 );
    //---
    time_inn::windClockUPtr = std::make_unique<WindClock>();
    time_inn::windClockUPtr->init();
    time_inn::timerUPtr->insert_update_functor( std::bind( &WindClock::update, time_inn::windClockUPtr.get() ) );

    esrc::insertState("time");
}

TimeBase &get_timer()noexcept{
    return *(time_inn::timerUPtr);
}


TimeCircle &get_logicTimeCircle()noexcept{
    return *(time_inn::logicTimeCircleUPtr);
}


WindClock &get_windClock()noexcept{
    return *(time_inn::windClockUPtr);
}



}//---------------------- namespace: esrc -------------------------//


