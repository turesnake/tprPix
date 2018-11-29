/*
 * ========================= tprTimer.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.03
 *                                        修改 -- 2018.11.03
 * ----------------------------------------------------------
 *   
 *   小型 时间管理器。
 *   依据 std::chrono::system_clock，一个精度为 microsec 的即时时钟
 * 
 * ----------------------------
 * 依赖的其它库：
 *       
 * ----------------------------
 */

#ifndef _TPR_TPR_TIMER_H_
#define _TPR_TPR_TIMER_H_

//------------ C -------------//
#include <ctime> 

//------------ CPP -------------//
#include <iostream> 
#include <string>
#include <vector> 

#include <ratio>  //- 编译器分数运算
#include <chrono> //- 时间库

//------------ libs -------------//

//------------ SELF -------------//



namespace tpr {//--------------- namespace: tpr -------------------//



class tprTimer{


public:
    using time_point_t = std::chrono::system_clock::time_point;
    using dura_t = std::chrono::system_clock::duration;
    using tick_t  = std::chrono::system_clock::rep;
                        //-- system_clock 使用的时钟精度似乎为 microsec

    //---- 构造函数 ----//
    explicit tprTimer() = default;

    void init(); //-- 所有 tprTimer 实例，必须init


    tprTimer::time_point_t now_timepoint(); 
                                //-- 获得当前时间点 timepoint 值。
    tprTimer::tick_t       now_ticks_since_epoch();     
                                //-- 获得当前时间点 ticks since epoch 值
    tprTimer::tick_t       now_ticks_since_init(); 
                                //-- 获得当前时间点 ticks since init 值


private:

    time_point_t timepoint_orig; //-- 类实例执行 init 时的 timepoint 值 

};


}//------------------- namespace: tpr ------------------------//
#endif

