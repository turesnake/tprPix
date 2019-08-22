/*
 * ========================= Camera.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "Camera.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"

//#include "tprDebug.h" //- tmp


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
    
    glm::dvec2 off { this->targetDPos.x - this->currentDPos.x, 
                    this->targetDPos.y - this->currentDPos.y };
    //-- 若非常接近，直接同步 --
    double criticalVal { 8.0 }; 
            //-- 适当提高临界值，会让 camera运动变的 “简练”
            // 同时利于 waterAnimCanvas 中的运算
    if( (std::abs(off.x)<=criticalVal) && (std::abs(off.y)<=criticalVal) ){
        this->targetDPos.x = this->currentDPos.x;
        this->targetDPos.y = this->currentDPos.y;
                            //- 注意 此2句 写法：在足够靠近时，camera放弃继续靠近，但此时并未对齐
        this->isMoving = false;
        return;
    }

    //---------------------------//
    //  将每一次 camera 的位移距离，都对齐于 windowsz 像素尺寸
    //---------------------------//    
    /*
    {
        double bili = ViewingBox::windowSZ.x / ViewingBox::gameSZ.x; // tmp
        //
        off.x = floor(bili * off.x) / bili;
        off.y = floor(bili * off.y) / bili;
    }
    */


    double alignX = this->approachPercent * off.x;
    double alignY = this->approachPercent * off.y;
    //-----------
    this->currentDPos.x += alignX;
    this->currentDPos.y += alignY;
    this->currentDPos.z =  -this->currentDPos.y + (0.5 * ViewingBox::z); //-- IMPORTANT --
    

    

}


/* ===========================================================
 *                     update_mat4_view    
 * -----------------------------------------------------------
 * --  生成 观察矩阵, 并将 观察矩阵 的指针 (const float *) 返回出去
 */
glm::mat4 &Camera::update_mat4_view(){

    this->mat4_view = glm::lookAt( glm_dvec3_2_vec3(this->currentDPos), 
                                (glm_dvec3_2_vec3(this->currentDPos) + cameraFront), 
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
    float ow = 0.5f * static_cast<float>(ViewingBox::gameSZ.x);  //- 横向边界半径（像素）
    float oh = 0.5f * static_cast<float>(ViewingBox::gameSZ.y);  //- 纵向边界半径（像素）

    //------ relative: zNear / zFar --------
    // 基于 currentDPos, 沿着 cameraFront 方向，推进 zNear_relative，此为近平面
    // 基于 currentDPos, 沿着 cameraFront 方向，推进 zFar_relative， 此为远平面
    // 两者都是 定值（无需每帧变化）
    float zNear_relative  = 0.0f;  //- 负数也接受
    float zFar_relative   = static_cast<float>(ViewingBox::z);

    this->mat4_projection = glm::ortho( -ow,   //-- 左边界
                                        ow,   //-- 右边界
                                        -oh,   //-- 下边界
                                        oh,   //-- 上边界
                                        zNear_relative, //-- 近平面
                                        zFar_relative  //-- 远平面
                                    );
        //-- 这组数据 其实可以被制作成 double ...

    return this->mat4_projection;
}

