/*
 * ========================= Camera.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CAMERA_H
#define TPR_CAMERA_H
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "vector_matrix.h" 
#include "ViewingBox.h"


class Camera{
public:
    Camera():
        targetDPos(glm::vec2(0.0, 0.0)),
        currentDPos(glm::vec3(  0.0,
                                0.0,
                                0.5 * ViewingBox::z ))
        {}

    void RenderUpdate();

    glm::mat4 &update_mat4_view();
    glm::mat4 &update_mat4_projection();


    //-- 瞬移到 某位置 --
    //   目前仅用于 sceneWorld 的 perpare 阶段
    inline void set_allDPos( const glm::dvec2 &tpos_ ){
        this->currentDPos.x = tpos_.x; 
        this->currentDPos.y = tpos_.y + 25.0; //- 不是完全对齐，而是留了段小距离来运动
        this->targetDPos  = tpos_;
        this->isMoving = true;
    }


    //- 外部代码控制 camera运动 的唯一方式
    inline void set_targetDPos( const glm::dvec2 &tpos_, double approachPercent_=0.08 ){
        if( tpos_ == this->targetDPos ){
            return;
        }
        this->targetDPos = tpos_;
        this->isMoving = true;
        this->approachPercent = approachPercent_;
    }

    //-- 由于 本游戏为 纯2D，所以不关心 camera 的 z轴值 --
    /*
    inline glm::vec2 get_camera2DFPos() const { 
        return  glm::vec2{ this->currentFPos.x, this->currentFPos.y };  
    }
    */
    inline glm::dvec2 get_camera2DDPos() const { 
        return  glm::dvec2{ this->currentDPos.x, 
                            this->currentDPos.y };  
    }

    //--- used for render DEEP_Z --
    //-- 注意，此处的 zNear/zFar 是相对世界坐标的 绝对值
    inline const double &get_zNear() const {
        return this->currentDPos.z;
    }
    inline double get_zFar() const {
        return this->currentDPos.z - ViewingBox::z; 
    }

private:
    //------ 观察／投影 矩阵 -----
    glm::mat4 mat4_view       = glm::mat4(1.0); //-- 观察矩阵，默认初始化为 单位矩阵
    glm::mat4 mat4_projection = glm::mat4(1.0); //-- 投影矩阵，默认初始化为 单位矩阵

    //------ 坐标向量 -------
    glm::dvec2 targetDPos; 
    glm::dvec3 currentDPos; 

    double  approachPercent {0.08};  //- camera运动的 “接近比率”

    //------ 方向向量 -------
    //-- 以下3个向量 都是 单位向量
    //-- 二维游戏的 摄像机视角是固定的，不需要每帧运算 --
    glm::vec3 cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; //-- 摄像机 观察的方向
    //glm::vec3 cameraRight { glm::vec3(1.0f, 0.0f, 0.0f) }; //-- 摄像机 右手 指向的方向。 未被使用...
    glm::vec3 cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f) }; //-- 摄像机 头顶向上 指向的方向。

    //glm::vec3 worldUp { glm::vec3(0.0f, 1.0f, 0.0f) };     //-- 世界轴的 Up方向，和 cameraUp 有区别。 未被使用...

    //===== flags =====//
    bool   isMoving       {true}; //- 是否在移动

};



#endif
