/*
 * ========================= GameObjPos.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *    专门服务于 GameObj 类型的 “pos数据集”
 *  ----
 *  - targetPos [MapCoord]
 *  - currentPos [glm::vec2]
 *  
 *  -----
 *  上诉数据之间的 转变变得复杂。
 *  将他们捆绑，集中存储在一个 数据容器中，统一管理
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_POS_H_
#define _TPR_GAME_OBJ_POS_H_

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "MapCoord.h"



class GameObjPos{
public:




private:
    //- 与 framePos 中的数据不同，下方2个pos 都是动态pos --
    glm::vec2  currentPos   {}; //- go.rootAnchor 在当前帧的 ppos
    MapCoord   targetPosOff {}; //- 从原来pos，到下一回合pos 的偏移差值
                                //  差值是最方便的，可以作用于每一个 colliEnt 
                                


};







#endif 

