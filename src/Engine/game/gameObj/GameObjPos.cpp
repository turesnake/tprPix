/*
 * ========================= GameObjPos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "GameObjPos.h"

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "MapEntCompass.h"

#include "debug.h"


namespace{//-------- namespace -----------//

    glm::vec2 halfMapEnt{   (float)HALF_PIXES_PER_MAPENT,
                            (float)HALF_PIXES_PER_MAPENT };
    
}//------------ namespace: end -----------//



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
    const MapEntCompass &compassRef = goPtr->get_rootAnchorPos().compass;
    IntVec2 p = currentMCPos.get_ppos() + compassRef.to_IntVec2();
    currentFPos.x = (float)p.x;
    currentFPos.y = (float)p.y;
}


/* ===========================================================
 *            align_currentFPos_by_currentMCPos
 * -----------------------------------------------------------
 * -- 将 currentFPos 对齐于 mapent坐标系
 */
void GameObjPos::align_currentFPos_by_currentMCPos(){

    const MapEntCompass &compassRef = goPtr->get_rootAnchorPos().compass;
    IntVec2 p = currentMCPos.get_ppos() + compassRef.to_IntVec2();
    currentFPos.x = (float)p.x;
    currentFPos.y = (float)p.y;
}


/* ===========================================================
 *             calc_rootAnchor_midFPos
 * -----------------------------------------------------------
 * -- 计算 go.rootAnchor 当前所在的 collient 的 midFPos
 */
glm::vec2 GameObjPos::calc_rootAnchor_midFPos(){

    const MapEntCompass &compassRef = goPtr->get_rootAnchorPos().compass;
    return ( currentFPos - compassRef.to_fpos() + halfMapEnt );
}





