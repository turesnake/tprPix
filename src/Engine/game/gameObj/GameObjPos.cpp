/*
 * ========================= GameObjPos.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.20
 *                                        修改 -- 2019.01.20
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "GameObjPos.h"

//-------------------- Engine --------------------//
#include "GameObj.h"


/* ===========================================================
 *                 init_by_currentMCPos
 * -----------------------------------------------------------
 * -- 将 go.goPos.currentFPos 放到 参数 _pos 指定的 mapCoordPos 上
 * -- 在调用此函数之前，应先确保 潜在碰撞区的 干净
 * -- 此函数不需要什么性能，反正不会经常设置 go的 mcpos
 */
void GameObjPos::init_by_currentMCPos( const MapCoord &_mcpos ){
    currentMCPos = _mcpos;
    //---
    const NineBox &compass = goPtr->get_rootAnchorPos().compass;
    IntVec2 p = currentMCPos.get_midPPos() + compass.to_IntVec2();
    currentFPos.x = (float)p.x;
    currentFPos.y = (float)p.y;
}






