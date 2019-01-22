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
class GameObj; //- tmp
class Move;
class GameObjPos;
class Collision;


//-- 想要控制一个 go的移动[爬行]，就应该 向其输入 crawlIns, 改写其 speedLevel
//-- 
class Crawl{
public:
    Crawl() = default;

    void init(  GameObj *_goPtr, 
                Move *_movePtr, 
                GameObjPos *_goPosPtr, 
                Collision *_collisionPtr ); //-- MUST --
    void RenderUpdate(); 

    inline void set_newCrawlDir( const NineBox &_newNB ){
        newNB = _newNB;
    }

private:
    GameObj     *goPtr    {nullptr};

    Move        *movePtr  {nullptr}; //- 每个 crawl实例 都属于一个 move实例, 强关联
    GameObjPos  *goPosPtr {nullptr}; 
    Collision   *collisionPtr {nullptr};                           
    
    //MapCoord  targetPos {};   //- 当前（及上一回合 设置的 目标pos（基于 go.rootAnchor）
                              //- 只在 节点帧 才被更新
                              // *** 此值可能被合并 ***

    NineBox  newNB     {0,0};  //- 本次渲染帧，新传入的 ci值（每一帧都被外部代码更新）
    NineBox  currentNB {0,0};  //- 当前正在处理的 ci值。（只在节点帧被改写）
    int   count  {};  //- 计数器，达到 max 后清零
    int   max    {};  //- count最大值， speeds[n].first
    float speed  {};  //- 当前帧的 位移速度（单轴）
};

#endif

