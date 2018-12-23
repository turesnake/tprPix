/*
 * ========================= Camera.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   摄像机 类
 * ----------------------------
 */

#ifndef _TPR_CAMERA_H_
#define _TPR_CAMERA_H_


/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>


//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//------------------- SELF --------------------//
#include "vector_matrix.h" 


class Camera{

public:
    //-- 构造函数  --
    //-- 参数 fov_ 是象征性的。
    Camera() 
        //:
        {}

    void init(); //-- 真正的初始化函数


    glm::mat4 &update_mat4_view();
    glm::mat4 &update_mat4_projection();

    //--- input: 鼠标位移 控制 摄像机运动 ---
    //void cameraPos_forward();
    //void cameraPos_back();
    void cameraPos_left();
    void cameraPos_right();
    void cameraPos_up();
    void cameraPos_down();

    void mousePos_move( double xpos, double ypos );
    void mouseFov_reset( double xoffset, double yoffset );

    //-- 由于 本游戏为 纯2D，所以不关心 camera 的 z轴值 --
    inline glm::vec2 get_camera2DPos()
                { return  glm::vec2{ cameraPos.x, cameraPos.y };  }


private:
    //------ 观察／投影 矩阵 -----
    glm::mat4 mat4_view;       //-- 观察矩阵，默认初始化为 单位矩阵
    glm::mat4 mat4_projection; //-- 投影矩阵，默认初始化为 单位矩阵

    //------ 坐标向量 -------
    glm::vec3 cameraPos { glm::vec3(0.0f, 0.0f, 1000.0f) }; 
                        //-- 摄像机 相对于世界的 位置。（世界坐标体系中）
                        //-- 初始值为，从世界中心 沿着 z轴正方向 后退 3.0f
    
    //------ 方向向量 -------
    //-- 以下3个向量 都是 单位向量
    glm::vec3 cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; //-- 摄像机 观察的方向
    glm::vec3 cameraRight; //-- 摄像机 右手 指向的方向。 
    glm::vec3 cameraUp;    //-- 摄像机 头顶向上 指向的方向。

    glm::vec3 worldUp { glm::vec3(0.0f, 1.0f, 0.0f) };     //-- 世界轴的 Up方向，和 cameraUp 有区别。

    //--------
    float cameraSpeed {}; //-- 移动速度. 中间值。

    float mouseX_off {};   //-- 当前帧 与 上一帧的 鼠标 x 轴位移 偏移量
    float mouseY_off {};   //-- 当前帧 与 上一帧的 鼠标 y 轴位移 偏移量
    float mouseX_last {};  //-- 上一帧的 鼠标 x 轴 位置（屏幕坐标,[800,600] 那个）
    float mouseY_last {};  //-- 上一帧的 鼠标 y 轴 位置（屏幕坐标,[800,600] 那个）

    float yaw {-90.0f}; //-- 偏航（角度），0.0 代表 摄像机正右方。-90.0 代表 摄像机 前方
    float pitch {}; //-- 俯仰（角度），0.0 代表 摄像机看向 地平线。

    int fst_mouse {1}; //-- 标志符，是否为 第一次 鼠标运动（游戏开始时）
                    //-- 若为 1，表示 是第一次
                    //-- 若为 0，表示 不是。

    float mousePos_sensitivity { 0.1f}; //-- 鼠标运动 修正值。

    //float fov {45.0f}; //-- Field of View／视野，初始化为 45.0f

    //---- funcs -----
    void update_camera_vectors(); //-- 刷新 camera 的几个核心向量。 
};


//-- 将 参数 cp 绑定为 当前摄像机
//void bind_camera_current( const Camera *cp );

//-- 获得 指向 当前摄像机 的指针
//Camera *camera_current();



#endif
