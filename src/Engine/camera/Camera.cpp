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
#include <math.h> //- abs

//-------------------- CPP --------------------//
#include <cassert>
#include <string>


//-------------------- Engine --------------------//
//#include "config.h" // SCR_WIDTH, SCR_HEIGHT
#include "windowConfig.h"
#include "srcs_engine.h" //- 所有资源

using std::string;


#include "debug.h" //- tmp


//---------------------- 局部 变量 ---------------------
namespace{
    Camera *camera_p_current = nullptr; //-- 当前摄像机 指针
}


/* ===========================================================
 *                        init    
 * -----------------------------------------------------------
 * --  真正的初始化函数
 */
void Camera::init(){

    currentPPos = glm::vec3( 0.0f,
                            0.0f,
                            0.5f * ViewingBox::z   
                            ); 

    targetPPos = glm::vec2( 0.0f, 0.0f );
}


/* ===========================================================
 *                  set_targetPos
 * -----------------------------------------------------------
 * -- 外部代码控制 camera运动 的唯一方式
 */
void Camera::set_targetPos( glm::vec2 _tpos, float _approachPercent ){

    if( _tpos == targetPPos ){
        return;
    }
    targetPPos = _tpos;
    isMoving = true;
    approachPercent = _approachPercent;
}


/* ===========================================================
 *                  RenderUpdate 
 * -----------------------------------------------------------
 * -- 每1渲染帧 执行的 update()
 * -- 目前只含有 运动系统
 */
void Camera::RenderUpdate(){

    if( isMoving == false ){
        return;
    }

    glm::vec2 off { targetPPos.x-currentPPos.x, 
                    targetPPos.y-currentPPos.y };
    //-- 若非常接近，直接同步 --
    if( (abs(off.x)<0.1f) && (abs(off.y)<0.1f) ){
        targetPPos.x = currentPPos.x;
        targetPPos.y = currentPPos.y;
        isMoving = false;
        return;
    }

    currentPPos.x += approachPercent * off.x;
    currentPPos.y += approachPercent * off.y;
    currentPPos.z =  -currentPPos.y + (0.5f * ViewingBox::z); //-- IMPORTANT --
}


/* ===========================================================
 *                     update_mat4_view    
 * -----------------------------------------------------------
 * --  生成 观察矩阵, 并将 观察矩阵 的指针 (const float *) 返回出去
 */
glm::mat4 &Camera::update_mat4_view(){

    mat4_view = glm::lookAt( currentPPos, 
                             (currentPPos + cameraFront), 
                             cameraUp );
    return mat4_view;
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
    // 基于 currentPPos, 沿着 cameraFront 方向，推进 zNear_relative，此为近平面
    // 基于 currentPPos, 沿着 cameraFront 方向，推进 zFar_relative， 此为远平面
    // 两者都是 定值（无需每帧变化）
    float zNear_relative  = 0.0f;  //- 负数也接受
    float zFar_relative   = ViewingBox::z;


    mat4_projection = glm::ortho( -ow,   //-- 左边界
                                   ow,   //-- 右边界
                                  -oh,   //-- 下边界
                                   oh,   //-- 上边界
                                   zNear_relative, //-- 近平面
                                   zFar_relative  //-- 远平面
                                );
    return mat4_projection;
}


/* ===========================================================
 *                print_pos    [debug]
 * -----------------------------------------------------------
 */
void Camera::print_pos(){
    cout << "cameraPos: " << currentPPos.x 
        << ", " << currentPPos.y 
        << ", " << currentPPos.z
        << endl;
}


