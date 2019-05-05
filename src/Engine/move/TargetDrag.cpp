/*
 * ====================== TargetDrag.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "TargetDrag.h"

//-------------------- C --------------------//
#include <cmath>
#include <cassert>

//-------------------- Engine --------------------//
#include "GameObj.h" 
#include "GameObjPos.h"
#include "Move.h" 



/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void TargetDrag::init(  Move *_movePtr, 
                        GameObjPos *_goPosPtr ){
    this->movePtr  = _movePtr;
    this->goPosPtr = _goPosPtr;
}



/* ===========================================================
 *                     RenderUpdate
 * -----------------------------------------------------------
 */
void TargetDrag::RenderUpdate(){

    if( this->isMoving == false ){
        return;
    }

    float criticalVal = 1.0;  //- 临界值
    float  approachPercent = 0.28; //- go运动的 “接近比率”
                                   // 在未来，会根据 move.speedLvl 来修改

    const glm::vec2 &currentFPos = this->goPosPtr->get_currentFPos();
    glm::vec2 off = this->targetFPos - currentFPos;
    //-- 若非常接近，直接同步 --
    if( (abs(off.x)<=criticalVal) && (abs(off.y)<=criticalVal) ){
        this->goPosPtr->set_by_currentFPos( this->targetFPos );
                                //- 注意，此处的写法和 camera 中不同
        this->isMoving = false;
        return;
    }

    this->goPosPtr->accum_currentFPos( off * approachPercent );
    //- 并不设置 z 轴值
}





