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

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "NineBox.h" 


//-- 依赖代码 --
class GameObj;
class Move;



//-- 想要控制一个 go的移动[爬行]，就应该 向其输入 crawlIns, 改写其 speedLevel
//-- 
class Crawl{
public:
    Crawl() = default;

    void init( GameObj *_goPtr, Move *_movePtr ); //-- MUST --
    void RenderUpdate(); 

    inline void set_newCrawlDir( const NineBox &_newNB ){
        newNB = _newNB;
    }

private:
    GameObj  *goPtr {nullptr}; //- 每个 crawl实例 都属于一个 go实例
                                //  两者 强关联，共存亡
    Move     *movePtr {nullptr}; 

    NineBox  newNB     {0,0};  //- 本次渲染帧，新传入的 ci值（每一帧都被外部代码更新）
    NineBox  currentNB {0,0};  //- 当前正在处理的 ci值。（只在节点帧被改写）
    int   count  {};  //- 计数器，达到 max 后清零
    int   max    {};  //- count最大值， speeds[n].first
    float speed  {};  //- 当前帧的 位移速度（单轴）
};

#endif

