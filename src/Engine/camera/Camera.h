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
#include "ViewingBox.h"


class Camera{
public:
    Camera() = default;

    void init();
    void RenderUpdate();

    glm::mat4 &update_mat4_view();
    glm::mat4 &update_mat4_projection();

    //- 外部代码控制 camera运动 的唯一方式
    void set_targetPos( glm::vec2 _tpos, float _approachPercent=0.08f ); 


    //-- 由于 本游戏为 纯2D，所以不关心 camera 的 z轴值 --
    inline const glm::vec2 get_camera2DPPos() const { 
        return  glm::vec2{ currentPPos.x, currentPPos.y };  
    }

    //--- used for render DEEP_Z --
    //-- 注意，此处的 zNear/zFar 是相对世界坐标的 绝对值
    inline const float get_zNear() const {
        return currentPPos.z;
    }
    inline const float get_zFar() const {
        return currentPPos.z - ViewingBox::z; 
    }

    void print_pos();//-- debug --
private:
    //------ 观察／投影 矩阵 -----
    glm::mat4 mat4_view;       //-- 观察矩阵，默认初始化为 单位矩阵
    glm::mat4 mat4_projection; //-- 投影矩阵，默认初始化为 单位矩阵

    //------ 坐标向量 -------
    glm::vec2 targetPPos  {}; 
    glm::vec3 currentPPos {}; 

                        
    bool   isMoving       {true}; //- 是否在移动
    float  approachPercent {0.08f};  //- camera运动的 “接近比率”

    
    //------ 方向向量 -------
    //-- 以下3个向量 都是 单位向量
    //-- 二维游戏的 摄像机视角是固定的，不需要每帧运算 --
    glm::vec3 cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; //-- 摄像机 观察的方向
    glm::vec3 cameraRight { glm::vec3(1.0f, 0.0f, 0.0f) }; //-- 摄像机 右手 指向的方向。 
    glm::vec3 cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f) }; //-- 摄像机 头顶向上 指向的方向。

    glm::vec3 worldUp { glm::vec3(0.0f, 1.0f, 0.0f) };     //-- 世界轴的 Up方向，和 cameraUp 有区别。


};



#endif
