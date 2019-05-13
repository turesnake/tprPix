/*
 * ========================= Crawl.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动: 爬行
 * ----------------------------
 */
#ifndef _TPR_CRAWL_H_
#define _TPR_CRAWL_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "DirAxes.h"


//-- 依赖代码 --
class GameObj; //- tmp
class Move;
class GameObjPos;
class Collision;


//-- 想要控制一个 go的移动[爬行]，就应该 向其输入 crawlIns, 改写其 speedLevel
//-- 
/*
class Crawl{
public:
    Crawl() = default;

    void init(  GameObj *_goPtr, 
                Move *_movePtr, 
                GameObjPos *_goPosPtr, 
                Collision *_collisionPtr ); //-- MUST --

    void RenderUpdate_2(); //- 新版，测试

    //void set_newCrawlDirAxes( const DirAxes &_newDirAxes );

private:
    GameObj     *goPtr    {nullptr};

    Move        *movePtr  {nullptr}; //- 每个 crawl实例 都属于一个 move实例, 强关联
    GameObjPos  *goPosPtr {nullptr}; 
    Collision   *collisionPtr {nullptr};                           
    

    DirAxes  newDirAxes     {};  //- 本次渲染帧，新传入的 方向值（每一帧都被外部代码更新）
    DirAxes  currentDirAxes {};  //- 当前正在处理的  方向值。（只在节点帧被改写）
};
*/

#endif

