/*
 * ========================= Camera.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   摄像机 类
 * ----------------------------
 */
#include "Camera.h"

//-------------------- C --------------------//
#include <cmath>
#include <cassert>

//-------------------- Engine --------------------//
#include "windowConfig.h"

//#include "debug.h" //- tmp

//---------------------- 局部 变量 ---------------------
namespace{
    Camera *camera_p_current = nullptr; //-- 当前摄像机 指针
}


/* ===========================================================
 *                        init    [pure]
 * -----------------------------------------------------------
 * --  真正的初始化函数
 */
void Camera::init(){

    this->currentFPos = glm::vec3(  0.0f,
                                    0.0f,
                                    0.5f * ViewingBox::z ); 

    this->targetPPos = glm::vec2( 0.0f, 0.0f );
}


/* ===========================================================
 *                  set_targetPos
 * -----------------------------------------------------------
 * -- 外部代码控制 camera运动 的唯一方式
 */
void Camera::set_targetPos( glm::vec2 _tpos, float _approachPercent ){

    if( _tpos == this->targetPPos ){
        return;
    }
    this->targetPPos = _tpos;
    this->isMoving = true;
    this->approachPercent = _approachPercent;
}


/* ===========================================================
 *                  RenderUpdate 
 * -----------------------------------------------------------
 * -- 每1渲染帧 执行的 update()
 * -- 目前只含有 运动系统
 */
void Camera::RenderUpdate(){

    if( this->isMoving == false ){
        return;
    }

    glm::vec2 off { this->targetPPos.x - this->currentFPos.x, 
                    this->targetPPos.y - this->currentFPos.y };
    //-- 若非常接近，直接同步 --
    float criticalVal = 2.0; 
            //-- 适当提高临界值，会让 camera运动变的 “简练”
            // 同时利于 waterAnimCanvas 中的运算
    if( (abs(off.x)<=criticalVal) && (abs(off.y)<=criticalVal) ){
        this->targetPPos.x = this->currentFPos.x;
        this->targetPPos.y = this->currentFPos.y;
        this->isMoving = false;
        return;
    }

    //---------------------------//
    //  为了解决游戏中 “chunk间白线” 问题
    //  限制 camera 每帧位移，都取整于 0.01 
    //---------------------------//    
    float alignX = this->approachPercent * off.x;
    float alignY = this->approachPercent * off.y;
    alignX = floor(alignX*100.0) / 100.0;
    alignY = floor(alignY*100.0) / 100.0;
    //-----------
    this->currentFPos.x += alignX;
    this->currentFPos.y += alignY;
    this->currentFPos.z =  -this->currentFPos.y + (0.5f * ViewingBox::z); //-- IMPORTANT --
}


/* ===========================================================
 *                     update_mat4_view    
 * -----------------------------------------------------------
 * --  生成 观察矩阵, 并将 观察矩阵 的指针 (const float *) 返回出去
 */
glm::mat4 &Camera::update_mat4_view(){

    this->mat4_view = glm::lookAt( this->currentFPos, 
                                (this->currentFPos + cameraFront), 
                                cameraUp );
    return this->mat4_view;
}


/* ===========================================================
 *                update_mat4_projection  
 * -----------------------------------------------------------
 * --  生成 ortho 投影矩阵, 并将 投影矩阵 的指针 (const float *) 返回出去
 */
glm::mat4 &Camera::update_mat4_projection(){

    //-- 在未来，WORK_WIDTH／WORK_HEIGHT 会成为变量（随窗口尺寸而改变）
    //   所以不推荐，将 ow/oh 写成定值
    float ow = 0.5f * ViewingBox::x;  //- 横向边界半径（像素）
    float oh = 0.5f * ViewingBox::y;  //- 纵向边界半径（像素）

    //------ relative: zNear / zFar --------
    // 基于 currentFPos, 沿着 cameraFront 方向，推进 zNear_relative，此为近平面
    // 基于 currentFPos, 沿着 cameraFront 方向，推进 zFar_relative， 此为远平面
    // 两者都是 定值（无需每帧变化）
    float zNear_relative  = 0.0f;  //- 负数也接受
    float zFar_relative   = ViewingBox::z;


    this->mat4_projection = glm::ortho( -ow,   //-- 左边界
                                        ow,   //-- 右边界
                                        -oh,   //-- 下边界
                                        oh,   //-- 上边界
                                        zNear_relative, //-- 近平面
                                        zFar_relative  //-- 远平面
                                );
    return this->mat4_projection;
}

