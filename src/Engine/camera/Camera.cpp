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

#include "esrc_state.h"
#include "esrc_coordinate.h"


namespace camera_inn {//------------------ namespace: camera_inn ---------------------//

    //- renderScope off limit:
    double limit_top      {};
    double limit_bottom   {};
    double limit_left     {};
    double limit_right    {};

    bool isInit {false};

}//--------------------- namespace: camera_inn end ------------------------//


void Camera::init()noexcept{

    if( !camera_inn::isInit ){
        camera_inn::isInit = true;
        //---
        tprAssert( esrc::is_setState("viewingBox") );

        camera_inn::limit_top       = 600.0 + 100.0;
        camera_inn::limit_bottom    = -(600.0 + 500.0);
        camera_inn::limit_left      = -(960.0 + 200.0);
        camera_inn::limit_right     = 960.0 + 200.0;
                    // 为方便测试使用的版本
                    // 正式版中，应该使用 0.5 * ViewingBox::gameSZ
    }
}



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
    
    //-- no need to align to pix
    double alignX = this->approachPercent * off.x;
    double alignY = this->approachPercent * off.y;

        //-----------
    this->currentDPos.x += alignX;
    this->currentDPos.y += alignY;
    this->currentDPos.z =  -this->currentDPos.y + (0.5 * ViewingBox::z); //-- IMPORTANT --

    //--- debug ---//
    tprDebug::collect_cameraSpeed( glm::dvec2{alignX, alignY} );
}



glm::mat4 &Camera::update_mat4_view(){

    const auto &worldCoord = esrc::get_worldCoordRef();

    glm::dvec2 innDPos { this->currentDPos.x, this->currentDPos.y };
    glm::dvec2 outDPos = worldCoord.calc_outDPos( innDPos );

    glm::dvec3 tmpDPos {outDPos.x,
                        outDPos.y,
                        this->currentDPos.z };

    this->mat4_view = glm::lookAt(  glm_dvec3_2_vec3(tmpDPos), 
                                    (glm_dvec3_2_vec3(tmpDPos) + cameraFront), 
                                    cameraUp );
    return this->mat4_view;

}



FloatVec2 Camera::calc_canvasCFPos()const noexcept{

    //-- 测试，将世界坐标系，转换成 人造物坐标系
    const auto &worldCoord = esrc::get_worldCoordRef();
    glm::dvec2 innDPos {    this->currentDPos.x,
                            this->currentDPos.y };
    glm::dvec2 outDPos = worldCoord.calc_outDPos( innDPos );

    //-- [left-bottom] 
    // 计算 canvas 左下角 这步计算，不应放入 坐标系扭曲运算中
    float w = static_cast<float>( outDPos.x ) - (0.5f * static_cast<float>(ViewingBox::gameSZ.x));
    float h = static_cast<float>( outDPos.y ) - (0.5f * static_cast<float>(ViewingBox::gameSZ.y));

    return FloatVec2{   w / static_cast<float>(PIXES_PER_CHUNK),
                        h / static_cast<float>(PIXES_PER_CHUNK) };
    
}



// 即便是 active go，也无需全部执行 render
// 只有离 camera 足够近的 gomesh。才会被 render。剩余的一律省略
bool Camera::is_in_renderScope( const glm::dvec2 &dpos_ )const noexcept{

    const auto &worldCoord = esrc::get_worldCoordRef();

    glm::dvec2 off = worldCoord.calc_outDPos( dpos_ - this->get_camera2DDPos() );

    if( (off.x > camera_inn::limit_left) && (off.x < camera_inn::limit_right) && 
        (off.y > camera_inn::limit_bottom) && (off.y < camera_inn::limit_top) ){
        return true;
    }else{
        return false;
    }
}



