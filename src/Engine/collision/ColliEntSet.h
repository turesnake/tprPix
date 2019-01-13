/*
 * ========================= ColliEntSet.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.13
 *                                        修改 -- 2019.01.13
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#ifndef _TPR_COLLI_ENT_SET_H_
#define _TPR_COLLI_ENT_SET_H_

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "MapCoord.h"
#include "PixVec.h"



class ColliEntSet{
public:

    //-- 也许要在未来该用 set 做容器 --
    std::vector<MapCoord>  colliEntOffs {}; 

    PixVec2   center  {}; //- 碰撞区中心

    float   radius {}; //- 半径（通常是个手动设置的 粗略值，多用于 fly移动 ）
    




};




#endif 

