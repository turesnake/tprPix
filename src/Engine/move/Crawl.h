/*
 * ========================= Crawl.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动: 爬行
 * ----------------------------
 */
#ifndef _TPR_CRAWL_H_
#define _TPR_CRAWL_H_

//-------------------- Engine --------------------//
#include "CrossState.h" 

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-- 依赖代码 --
class GameObj;

//-- go 可设置的 8档 速度 --
enum class SpeedLevel : u8 {
    LV_1 = 1, //- 3.0   - 1
    LV_2 = 2, //- 1.5   - 2
    LV_3 = 3, //- 1.0   - 3
    LV_4 = 4, //- 0.75  - 4
    LV_5 = 5, //- 0.6   - 5
    LV_6 = 6, //- 0.5   - 6
    LV_7 = 7, //- 0.375 - 8
    LV_8 = 8  //- 0.3   - 10
};


//-- 想要控制一个 go的移动[爬行]，就应该 向其输入 crossState, 改写其 speedLevel
//-- 
class Crawl{
public:
    Crawl() = default;

    void init( GameObj *_goPtr ); //-- MUST --
    void RenderUpdate(); 

    //-- 被 player.onGameCross() 调用
    inline void set_newCrossState( const CrossState &_newCS ){
        newCS = _newCS;
    }

private:
    GameObj  *goPtr {nullptr}; //- 每个 move实例 都属于一个 go实例
                                //  两者 强关联，共存亡

    CrossState  newCS     {0,0};  //- 本次渲染帧，新传入的 cs值（每一帧都被外部代码更新）
    CrossState  currentCS {0,0};  //- 当前正在处理的 cs值。（只在节点帧被改写）
    int   count  {};  //- 计数器，达到 max 后清零
    int   max    {};  //- count最大值， speeds[n].first
    float speed  {};  //- 当前帧的 位移速度（单轴）
};

#endif

