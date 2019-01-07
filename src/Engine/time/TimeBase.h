/*
 * ========================= TimeBase.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.22
 *                                        修改 -- 2018.12.22
 * ----------------------------------------------------------
 *   基础时间类, singleton
 * ----------------------------
 */
#ifndef _TPR_TIME_BASE_H_
#define _TPR_TIME_BASE_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//------------------- C --------------------// 
#include <math.h> //- abs

//------------------- Libs --------------------// 
#include "tprDataType.h"


//--- 最基础的时间类,singleton ---//
class TimeBase{
public:

    explicit TimeBase() = default;


    //--- 在 每一主循环 中调用 ---//
    inline void update_time(){
        frames++;
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime; //-- 更新 deltaTime
        lastTime = currentTime;

        //---- smoothDeltaTime ----//
        //-- 游戏启动后，前10帧 不平滑 dt值 --
        if( frames < 10 ){
            smoothDeltaTime = deltaTime;
            return;
        }

        //-- 游戏正常后，开始 平滑 dt值 --
        float off = deltaTime - smoothDeltaTime;
        if( abs(off) <= step ){
            //-- 波动不大，直接对齐 --
            smoothDeltaTime = deltaTime;
        }else{
            //-- 波动过大，步进对齐 --
            (off > 0) ? smoothDeltaTime += step :
                        smoothDeltaTime -= step;
        }
    }

    //--- 获得 当前时间 （从 glfw 启动 开始计算）---//
    inline float get_currentTime(){
        return glfwGetTime();
    }

    //--- 获得 上一帧的 deltaTime ---//
    inline const float get_last_deltaTime() const {
        return deltaTime;
    }

    //--- 获得 平滑处理过的 deltaTime ---//
    // 注意，此处的 deltaTime 不是 “上一帧的 dt”，
    // 而是一个 平滑值。专门提供给 其它 模块使用 
    inline const float get_smoothDeltaTime() const {
        return smoothDeltaTime;
    }

    //-- 获得 游戏 总帧数 --//
    inline const u64 get_frames() const {
        return frames;
    }



private:
    u64  frames {0}; //- 游戏运行后的 总帧数

    //-- deltaTime --
    float currentTime {0.0f}; //-- 当前 时间值
    float lastTime    {0.0f}; //-- 上一帧的 时间值
    float deltaTime   {0.0f}; //-- 当前帧 与 上一帧的 时间差

    //-- smoothDeltaTime --
    float smoothDeltaTime {0.0f}; //-- 平滑处理过的 dt值。专用于其他模块
    float step            {0.001f}; //- 平滑 步进值
    
};






#endif
