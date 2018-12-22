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

//-------------------- CPP --------------------//
#include <cassert> //- assert
#include <string>
#include <iostream>

//------------------- SELF --------------------//
#include "config.h" // SCR_WIDTH, SCR_HEIGHT
//#include "glob_gl.h" //- get_deltaTime,

#include "srcs_manager.h" //- 所有资源


using std::cout;
using std::endl;
using std::string;

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

    update_camera_vectors();

    //----------------------//
    //  生成 mat4_projection
    //----------------------//
    float ow = (float)(WORK_WIDTH/2);  //- 横向边界半径（像素）
    float oh = (float)(WORK_HEIGHT/2); //- 纵向边界半径（像素）

    mat4_projection = glm::ortho( -ow,   //-- 左边界
                                   ow,   //-- 右边界
                                  -oh,   //-- 下边界
                                   oh,   //-- 上边界
                                  -100.0f, //-- 近平面
                                  2000.0f  //-- 远平面
                                );

}


/* ===========================================================
 *                     update_mat4_view    
 * -----------------------------------------------------------
 * --  生成 观察矩阵, 并将 观察矩阵 的指针 (const float *) 返回出去
 */
glm::mat4 &Camera::update_mat4_view(){

    mat4_view = glm::lookAt( cameraPos, 
                             (cameraPos + cameraFront), 
                             cameraUp );

    return mat4_view;
}


/* ===========================================================
 *                update_mat4_projection  
 * -----------------------------------------------------------
 * --  生成 ortho 投影矩阵, 并将 投影矩阵 的指针 (const float *) 返回出去
 * --  
 */
glm::mat4 &Camera::update_mat4_projection(){
        
    //-----------------------------//
    //  已在 init 阶段生成，不需要改写
    //-----------------------------//
    return mat4_projection;
}


/* ===========================================================
 *           cameraPos_up / down / left / right   
 * -----------------------------------------------------------
 * --  通过 wsad 键，控制摄像机 前后左右移动
 */
void Camera::cameraPos_forward(){
    /*
    cameraSpeed = 2.5f * get_deltaTime();
    cameraPos += cameraFront * cameraSpeed;
    */
}


void Camera::cameraPos_back(){
    /*
    cameraSpeed = 2.5f * get_deltaTime();
    cameraPos -= cameraFront * cameraSpeed;
    */
}


void Camera::cameraPos_left(){
    cameraSpeed = 30.0f * timer.get_last_deltaTime();
    cameraPos -= cameraRight * cameraSpeed;
}


void Camera::cameraPos_right(){
    cameraSpeed = 30.0f * timer.get_last_deltaTime();
    cameraPos += cameraRight * cameraSpeed;
}


void Camera::cameraPos_up(){
    cameraSpeed = 30.0f * timer.get_last_deltaTime();
    cameraPos += cameraUp * cameraSpeed;
}

void Camera::cameraPos_down(){
    cameraSpeed = 30.0f * timer.get_last_deltaTime();
    cameraPos -= cameraUp * cameraSpeed;
}


/* ===========================================================
 *                    mousePos_move     
 * -----------------------------------------------------------
 * -- 鼠标位移，控制 摄像机 视角。
 */
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

    //--- 因为 camera 姿态发生改变，需要更新 几个核心向量。
    update_camera_vectors(); 
}


/* ===========================================================
 *                     mouseFov_reset    
 * -----------------------------------------------------------
 * -- 鼠标滚轮，控制 视角 fov ／ field of view
 */
/*
void Camera::mouseFov_reset( double xoffset, double yoffset ){

    if( fov >= 1.0f && fov <= 45.0f ){
        fov -= yoffset;
    }

    if( fov <= 1.0f ){
        fov = 1.0f;
    }
    
    if( fov >= 45.0f ){
        fov = 45.0f;
    }
}
*/

/* ===========================================================
 *                 update_camera_vectors
 * -----------------------------------------------------------
 * -- 刷新 camera 的几个核心向量。 
 * -- 分别在 camera 初始化，以及 camera 视角转变（通过鼠标位移）时，
 * -- 本函数被执行
 */
void Camera::update_camera_vectors(){

    //-- 更新 camera 前视向量
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);

    //- 更新 camera 右手向量
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp)); 
    //- 更新 camera 头顶向量
    cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));   

}


/* ===========================================================
 *                      bind_camera_current   
 * -----------------------------------------------------------
 * --  将 参数 cp 绑定为 当前摄像机
 */
void bind_camera_current( const Camera *cp ){
    camera_p_current = const_cast<Camera*>( cp );
}

/* ===========================================================
 *                       camera_current
 * -----------------------------------------------------------
 * --  获得 指向 当前摄像机 的指针
 */
Camera *camera_current(){

    if( camera_p_current == nullptr ){
        cout << "Camera::camera_current: error. \n"
             << "camera_p_current = nullptr"
             << endl;
        assert(0);
    }

    return camera_p_current;
}














