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
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

#include "pch.h"

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "FloatVec.h"


class Camera{
public:
    Camera():
        targetDPos(glm::dvec2(0.0, 0.0)),
        currentDPos(glm::dvec2(0.0, 0.0)),
        renderDPos(glm::dvec3(0.0, 0.0, ViewingBox::halfZ ))
        {
            this->init();
        }

    

    void RenderUpdate();

    bool is_in_renderScope( const glm::dvec2 &dpos_ )const noexcept;

    glm::mat4 &update_mat4_view();

    
    inline glm::mat4 &update_mat4_projection(){
        if( !this->isProjectionSet ){
            this->isProjectionSet = true; // only once
        }
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
                                            zNear_relative,  //-- 近平面
                                            zFar_relative ); //-- 远平面
        return this->mat4_projection;
    }
    


    //-- 瞬移到 某位置 --
    //   目前仅用于 sceneWorld 的 perpare 阶段
    inline void set_allDPos( const glm::dvec2 &tpos_ )noexcept{
        this->currentDPos.x = tpos_.x; 
        this->currentDPos.y = tpos_.y + 25.0; //- 不是完全对齐，而是留了段小距离来运动
        this->targetDPos  = tpos_;
        this->isMoving = true;
    }


    //- 外部代码控制 camera运动 的唯一方式
    inline void set_targetDPos( const glm::dvec2 &tpos_, double approachPercent_=0.1 )noexcept{
        if( tpos_ == this->targetDPos ){
            return;
        }
        this->targetDPos = tpos_;
        this->isMoving = true;
        this->approachPercent = approachPercent_;
    }


    inline glm::dvec2 get_currentDPos() const noexcept{  
        return this->currentDPos;
    }

    //--- used for render DEEP_Z --
    //-- 注意，此处的 zNear/zFar 是相对世界坐标的 绝对值
    inline double   get_zNear()const noexcept{ return this->renderDPos.z; }
    inline double   get_zFar()const noexcept{ return (this->renderDPos.z - ViewingBox::z); }


    inline bool     get_isMoving()const noexcept{ return this->isMoving; }
    inline bool     get_isProjectionSet()const noexcept{ return this->isProjectionSet; }

    //-- only return true in first frame --
    inline bool get_isFirstFrame()noexcept{ 
        if( this->isFirstFrame ){
            this->isFirstFrame = false;
            return true;
        }
        return false;
    }

    inline glm::vec2 calc_canvasFPos()const noexcept{        
        float w = static_cast<float>(this->currentDPos.x) - (0.5f * static_cast<float>(ViewingBox::gameSZ.x));
        float h = static_cast<float>(this->currentDPos.y) - (0.5f * static_cast<float>(ViewingBox::gameSZ.y));
        return glm::vec2{ w, h };
    }

    //-- canvas.chunk_fpos [left-bottom] 
    FloatVec2 calc_canvasCFPos()const noexcept;
   

private:
    void init()noexcept;

    //------ 观察／投影 矩阵 -----
    glm::mat4 mat4_view       = glm::mat4(1.0); //-- 观察矩阵，默认初始化为 单位矩阵
    glm::mat4 mat4_projection = glm::mat4(1.0); //-- 投影矩阵，默认初始化为 单位矩阵

    //------ 坐标向量 -------
    //- in worldCoord -
    glm::dvec2 targetDPos; 
    glm::dvec2 currentDPos;
    //- in renderCoord/windowCoord -
    glm::dvec3 renderDPos;

    double  approachPercent {0.1};  //- camera运动的 “接近比率”

    //------ 方向向量 -------
    //-- 以下3个向量 都是 单位向量
    //-- 二维游戏的 摄像机视角是固定的，不需要每帧运算 --
    glm::vec3 cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; //-- 摄像机 观察的方向
    //glm::vec3 cameraRight { glm::vec3(1.0f, 0.0f, 0.0f) }; //-- 摄像机 右手 指向的方向。 未被使用...
    glm::vec3 cameraUp    { glm::vec3(0.0f, 1.0f, 0.0f) }; //-- 摄像机 头顶向上 指向的方向。

    //glm::vec3 worldUp { glm::vec3(0.0f, 1.0f, 0.0f) };     //-- 世界轴的 Up方向，和 cameraUp 有区别。 未被使用...

    //===== flags =====//
    bool   isMoving        {true}; //- 是否在移动
    bool   isFirstFrame    {true};  //- 游戏第一帧
    bool   isProjectionSet {false}; //- 目前游戏中，mat4_projection 只需要被传入gl一次 ...
};

#endif

