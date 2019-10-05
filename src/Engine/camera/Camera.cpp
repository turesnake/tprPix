/*
 * ======================== Camera.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Camera.h"


//-------------------- Engine --------------------//
#include "speedLog.h"



void Camera::RenderUpdate(){

    if( this->isMoving == false ){
        return;
    }
        
    glm::dvec2 off { this->targetDPos.x - this->currentDPos.x, 
                    this->targetDPos.y - this->currentDPos.y };
    //-- 若非常接近，直接同步 --
    double criticalVal { 8.0 }; 
                //-- 适当提高临界值，会让 camera运动变的 “简练”
                // 同时利于 waterAnimCanvas 中的运算
    if( (std::abs(off.x)<=criticalVal) && (std::abs(off.y)<=criticalVal) ){
        this->targetDPos.x = this->currentDPos.x;
        this->targetDPos.y = this->currentDPos.y;
                                //- 在足够靠近时，camera放弃继续靠近，但此时并未对齐
        this->isMoving = false;
        return;
    }

    double alignX = this->approachPercent * off.x;
    double alignY = this->approachPercent * off.y;
        //-----------
    this->currentDPos.x += alignX;
    this->currentDPos.y += alignY;
    this->currentDPos.z =  -this->currentDPos.y + (0.5 * ViewingBox::z); //-- IMPORTANT --

    //--- debug ---//
    tprDebug::collect_cameraSpeed( glm::dvec2{alignX, alignY} );
}







