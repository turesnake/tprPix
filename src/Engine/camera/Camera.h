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
//=== *** glad FIRST, glfw SECEND *** ===
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

//-------------------- Engine --------------------//
#include "vector_matrix.h" 


class Camera{
public:
    Camera() = default;

    void init();
    void RenderUpdate();

    glm::mat4 &update_mat4_view();
    glm::mat4 &update_mat4_projection();

    //--- input: 鼠标位移 控制 摄像机运动 ---
    //void cameraPos_forward();
    //void cameraPos_back();
    //void cameraPos_left();
    //void cameraPos_right();
    //void cameraPos_up();
    //void cameraPos_down();

    //- 外部代码控制 camera运动 的唯一方式
    void set_targetPos( glm::vec2 _tpos, float _approachPercent=0.08f ); 

    //void mousePos_move( double xpos, double ypos );

    //-- 由于 本游戏为 纯2D，所以不关心 camera 的 z轴值 --
    inline const glm::vec2 get_camera2DPos() const { 
        return  glm::vec2{ currentPos.x, currentPos.y };  
    }

    //--- used for render DEEP_Z --
    //-- 注意，此处的 zNear/zFar 是相对世界坐标的 绝对值
    inline const float get_zNear() const {
        return currentPos.z;
    }
    inline const float get_zFar() const {
        return currentPos.z - boxSize.z; 
    }

    void print_pos();//-- debug --
private:
    //------ 观察／投影 矩阵 -----
    glm::mat4 mat4_view;       //-- 观察矩阵，默认初始化为 单位矩阵
    glm::mat4 mat4_projection; //-- 投影矩阵，默认初始化为 单位矩阵

    //------ 坐标向量 -------
    glm::vec2 targetPos  {}; 
    glm::vec3 currentPos {}; 

    glm::vec3  boxSize  {};  //-- 取景盒 xyz直径。based on pixel
                        
    bool   is_moving       {true}; //- 是否在移动
    float  approachPercent {0.08f};  //- camera运动的 “接近比率”

    
    //------ 方向向量 -------
    //-- 以下3个向量 都是 单位向量
    //-- 二维游戏的 摄像机视角是固定的，不需要每帧运算 --
    glm::vec3 cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; //-- 摄像机 观察的方向
    glm::vec3 cameraRight { glm::vec3(1.0f, 0.0f, 0.0f) }; //-- 摄像机 右手 指向的方向。 
    glm::vec3 cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f) }; //-- 摄像机 头顶向上 指向的方向。

    glm::vec3 worldUp { glm::vec3(0.0f, 1.0f, 0.0f) };     //-- 世界轴的 Up方向，和 cameraUp 有区别。

    //----- mouse -----
    //float cameraSpeed {}; //-- 移动速度. 中间值。
    //float mouseX_off {};   //-- 当前帧 与 上一帧的 鼠标 x 轴位移 偏移量
    //float mouseY_off {};   //-- 当前帧 与 上一帧的 鼠标 y 轴位移 偏移量
    //float mouseX_last {};  //-- 上一帧的 鼠标 x 轴 位置（屏幕坐标,[800,600] 那个）
    //float mouseY_last {};  //-- 上一帧的 鼠标 y 轴 位置（屏幕坐标,[800,600] 那个）
    //float yaw {-90.0f}; //-- 偏航（角度），0.0 代表 摄像机正右方。-90.0 代表 摄像机 前方
    //float pitch {}; //-- 俯仰（角度），0.0 代表 摄像机看向 地平线。
    //int fst_mouse {1}; //-- 标志符，是否为 第一次 鼠标运动（游戏开始时）
                    //-- 若为 1，表示 是第一次
                    //-- 若为 0，表示 不是。
    //float mousePos_sensitivity { 0.1f}; //-- 鼠标运动 修正值。


    //---- funcs ----- 
};



#endif
