/*
 * ========================= Camera.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   摄像机 类
 * ----------------------------
 */

#include "Camera.h"

//-------------------- C --------------------//
#include <math.h> //- abs

//-------------------- CPP --------------------//
#include <cassert> //- assert
#include <string>


//-------------------- Engine --------------------//
#include "config.h" // SCR_WIDTH, SCR_HEIGHT
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

    boxSize = glm::vec3( (float)WORK_WIDTH,
                         (float)WORK_HEIGHT,
                         1000.0f   //-- tmp 
                        );  

    currentPos = glm::vec3( 0.0f,
                            0.0f,
                            0.5f * boxSize.z   
                            ); 

    targetPos = glm::vec2( 0.0f, 0.0f );
}


/* ===========================================================
 *                  set_targetPos
 * -----------------------------------------------------------
 * -- 外部代码控制 camera运动 的唯一方式
 */
void Camera::set_targetPos( glm::vec2 _tpos, float _approachPercent ){

    if( _tpos == targetPos ){
        return;
    }
    targetPos = _tpos;
    is_moving = true;
    approachPercent = _approachPercent;
}


/* ===========================================================
 *                  RenderUpdate 
 * -----------------------------------------------------------
 * -- 每1渲染帧 执行的 update()
 * -- 目前只含有 运动系统
 */
void Camera::RenderUpdate(){

    if( is_moving == false ){
        return;
    }

    glm::vec2 off { targetPos.x-currentPos.x, 
                    targetPos.y-currentPos.y };
    //-- 若非常接近，直接同步 --
    if( (abs(off.x)<0.1f) && (abs(off.y)<0.1f) ){
        targetPos.x = currentPos.x;
        targetPos.y = currentPos.y;
        is_moving = false;
        return;
    }

    currentPos.x += approachPercent * off.x;
    currentPos.y += approachPercent * off.y;
    currentPos.z =  -currentPos.y + (0.5f * boxSize.z); //-- IMPORTANT --
}


/* ===========================================================
 *                     update_mat4_view    
 * -----------------------------------------------------------
 * --  生成 观察矩阵, 并将 观察矩阵 的指针 (const float *) 返回出去
 */
glm::mat4 &Camera::update_mat4_view(){

    mat4_view = glm::lookAt( currentPos, 
                             (currentPos + cameraFront), 
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
    float ow = 0.5f * boxSize.x;  //- 横向边界半径（像素）
    float oh = 0.5f * boxSize.y;  //- 纵向边界半径（像素）

    //------ relative: zNear / zFar --------
    // 基于 currentPos, 沿着 cameraFront 方向，推进 zNear_relative，此为近平面
    // 基于 currentPos, 沿着 cameraFront 方向，推进 zFar_relative， 此为远平面
    // 两者都是 定值（无需每帧变化）
    float zNear_relative  = 0.0f;  //- 负数也接受
    float zFar_relative   = boxSize.z;


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
    cout << "cameraPos: " << currentPos.x 
        << ", " << currentPos.y 
        << ", " << currentPos.z
        << endl;
}


/* ===========================================================
 *           cameraPos_up / down / left / right   
 * -----------------------------------------------------------
 * --
 */
/*
void Camera::cameraPos_left(){
    cameraSpeed = 30.0f * esrc::timer.get_last_deltaTime();
    currentPos -= cameraRight * cameraSpeed;
}
void Camera::cameraPos_right(){
    cameraSpeed = 30.0f * esrc::timer.get_last_deltaTime();
    currentPos += cameraRight * cameraSpeed;
}
void Camera::cameraPos_up(){
    cameraSpeed = 30.0f * esrc::timer.get_last_deltaTime();
    currentPos += cameraUp * cameraSpeed;
}
void Camera::cameraPos_down(){
    cameraSpeed = 30.0f * esrc::timer.get_last_deltaTime();
    currentPos -= cameraUp * cameraSpeed;
}
*/


/* ===========================================================
 *                    mousePos_move     
 * -----------------------------------------------------------
 * -- 鼠标位移，控制 摄像机 视角。
 */
/*
void Camera::mousePos_move( double xpos, double ypos ){

    //--- 游戏开始时，第一次鼠标运动 时的 配置。
    if( 1 == fst_mouse ){
        mouseX_last = xpos;
        mouseY_last = ypos;
        fst_mouse = 0;
    }

    //---- 更新 鼠标坐标
    mouseX_off = xpos - mouseX_last;
    mouseY_off = -(ypos - mouseY_last); //-- 反向 一下。

    mouseX_last = xpos;
    mouseY_last = ypos;

    //---- 
    mouseX_off *= mousePos_sensitivity;
    mouseY_off *= mousePos_sensitivity;

    yaw   += mouseX_off; //-- 偏航（角度）
    pitch += mouseY_off; //-- 俯仰（角度）

    //------ 避免 俯仰值 到达 90度 （看向天顶）
    if( pitch > 89.0f ){
        pitch = 89.0f;
    }
    if( pitch < -89.0f ){
        pitch = -89.0f;
    }
}
*/




