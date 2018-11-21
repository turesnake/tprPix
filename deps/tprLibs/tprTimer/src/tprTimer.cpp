/*
 * ======================= tprTimer.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.03
 *                                        修改 -- 2018.11.03
 * ----------------------------------------------------------
 *   
 *  时间管理器。
 * 
 * ----------------------------
 * 依赖的其它库：
 *       
 * ----------------------------
 */
#include "tprTimer.h"



namespace tpr {//--------------- namespace: tpr -------------------//


/* ===========================================================
 *                           init
 * -----------------------------------------------------------
 */
void tprTimer::init(){

    timepoint_orig = now_timepoint();
}

/* ===========================================================
 *                  now_timepoint
 * -----------------------------------------------------------
 * -- 获得当前时间点 timepoint 值。
 */
tprTimer::time_point_t tprTimer::now_timepoint(){

    return std::chrono::system_clock::now();
}


/* ===========================================================
 *                    now_ticks_since_epoch
 * -----------------------------------------------------------
 * -- 获得当前时间点 ticks since epoch 值
 * -- 从 1970年1月1日 凌晨1点 到现在的 ticks
 */
tprTimer::tick_t tprTimer::now_ticks_since_epoch(){

    dura_t du = now_timepoint().time_since_epoch();
    return du.count();
}

/* ===========================================================
 *                    now_ticks_since_init
 * -----------------------------------------------------------
 * -- 获得当前时间点 ticks since init 值
 * -- 从 本类实例执行 init，到现在的 ticks
 */
tprTimer::tick_t tprTimer::now_ticks_since_init(){

    dura_t off_du = now_timepoint() - timepoint_orig;
    return off_du.count();
}


}//------------------- namespace: tpr ------------------------//
