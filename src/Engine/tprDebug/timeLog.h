/*
 * ========================= timeLog.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_TIME_LOG_H
#define TPR_TIME_LOG_H

//------------------- CPP --------------------//
#include <map>

namespace tprDebug {//---------- namespace: tprDebug --------------//


void init_timeLog();
void collect_deltaTime(double deltaTime_);
void process_and_echo_timeLog();

size_t get_frameIdx();


}//-------------------- namespace: tprDebug end --------------//
#endif 

