/*
 * ========================= TimeBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   基础时间类
 * ----------------------------
 */
#ifndef TPR_TIME_BASE_H
#define TPR_TIME_BASE_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//------------------- CPP --------------------// 
#include <cmath>
#include <functional>
#include <vector>

//------------------- Engine --------------------// 
#include "timeLog.h"
#include "functorTypes.h"


//--- 最基础的时间类,singleton ---//
class TimeBase{
public:
    TimeBase()=default;

    //--- 在 每一主循环 起始处调用 ---//
    inline void update_before_all()noexcept{
        this->frameNum++;
        this->currentTime = glfwGetTime();
        this->deltaTime = this->currentTime - this->lastTime; //-- 更新 deltaTime
        this->lastTime = this->currentTime;

        //---- smoothDeltaTime ----//
        //-- 游戏启动后，前10帧 不平滑 dt值 --
        if( this->frameNum < 10 ){
            this->smoothDeltaTime = this->deltaTime;
            return;
        }

        //-- 游戏正常后，开始 平滑 dt值 --
        double off = this->deltaTime - this->smoothDeltaTime;
        if( std::abs(off) <= this->step ){
            //-- 波动不大，直接对齐 --
            this->smoothDeltaTime = this->deltaTime;
        }else{
            //-- 波动过大，步进对齐 --
            (off > 0) ? this->smoothDeltaTime += this->step :
                        this->smoothDeltaTime -= this->step;
        }
        //--- windClock ---
        for( auto &f : this->updates ){
            f();
        }

        //-----------
        tprDebug::collect_deltaTime(this->deltaTime); //- debug
    }


    inline void insert_update_functor( F_void update_ ){
        this->updates.push_back( update_ );
    }

    //--- 获得 当前时间 （从 glfw 启动 开始计算）---//
    //  目前仅被用于 random.cpp -> get_new_seed()
    inline double get_currentTime()noexcept{ return glfwGetTime(); }

    //--- 获得 上一帧的 deltaTime ---//
    //  ...目前未被任何代码使用...
    inline double get_last_deltaTime() const noexcept{ return this->deltaTime; }

    //--- 获得 平滑处理过的 deltaTime ---//
    // 注意，此处的 deltaTime 不是 “上一帧的 dt”，
    // 而是一个 平滑值。专门提供给 其它 模块使用 
    //  ...目前未被任何代码使用...
    inline double get_smoothDeltaTime() const noexcept{ return this->smoothDeltaTime; }

    inline uint64_t get_frameNum()const noexcept{ return this->frameNum; } //-- 获得 游戏 总帧数 --//

    //-- 依靠 db记录的 gameTime 旧值，来重启 gameTime 记录器 --
    inline void start_record_gameTime( double gameTime_from_db_ )noexcept{
        this->lastGameTime_from_db = gameTime_from_db_;
        this->begPoint_of_gameTime = this->currentTime;
    }

    //-- 获得当前时刻的 gameTime 值 --
    //   此函数可在 游戏运行期 的任何时刻 调用
    inline double get_gameTime() const noexcept{
        return (this->currentTime - this->begPoint_of_gameTime + 
                this->lastGameTime_from_db );
    }

    //===== static =====//
    static double logicUpdateTimeLimit;

private:

    std::vector<F_void> updates {}; // oth mods

    //----- vals -----//
    uint64_t  frameNum {0}; //- 游戏运行后的 总帧数

    //-- deltaTime --
    double currentTime {0.0}; //-- 当前 时间值
    double lastTime    {0.0}; //-- 上一帧的 时间值
    double deltaTime   {0.0}; //-- 当前帧 与 上一帧的 时间差

    //-- smoothDeltaTime --
    double smoothDeltaTime {0.0}; //-- 平滑处理过的 dt值。专用于其他模块
    double step            {0.001}; //- 平滑 步进值

    //-- gameTime --
    // gameTime : 游戏运行总时间
    // 这个值并不真的存储，而是每次 读取时(get) 临时计算一个。
    double  lastGameTime_from_db  {0.0}; //- 从db读取的，gameTime 值
    double  begPoint_of_gameTime  {0.0}; //- 当玩家选中一个存档时，会记下此时的 currentTime
                                         //- 作为 本局游戏的 gameTime 起始值
    
};

inline double TimeBase::logicUpdateTimeLimit {1.0/60.0};


#endif

