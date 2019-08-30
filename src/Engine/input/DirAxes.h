/*
 * ========================= DirAxes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   InputINS 中 表达 方向的 数据
 *   当数据源自 joystick 时，
 *      值直接为 axis_0, axis_1
 *   当数据源自 键盘
 *      值被设置为 (-1.0, 0.0, 1.0) 三种之一
 * ----------------------------
 */
#ifndef TPR_DIR_AXES_H
#define TPR_DIR_AXES_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"


class DirAxes{
public:
    DirAxes() = default;
    DirAxes( double x_, double y_ ):
        x(x_),
        y(y_)
        {
            tprAssert( (x_>=-1.0) && (x_<=1.0) &&
                    (y_>=-1.0) && (y_<=1.0) );
            this->consider_threshold_x();
            this->consider_threshold_y();
        }

    inline void clear_all(){
        this->x = 0.0;
        this->y = 0.0;
    }

    inline void set( double x_, double y_ ){
        tprAssert( (x_>=-1.0) && (x_<=1.0) &&
                (y_>=-1.0) && (y_<=1.0) );
        this->x = x_;
        this->y = y_;
        this->consider_threshold_x();
        this->consider_threshold_y();
    }

    //-- 仅用于 InputINS --
    inline void set_x( double x_ ){
        tprAssert( (x_>=-1.0) && (x_<=1.0) );
        this->x = x_;
        this->consider_threshold_x();
    }
    inline void set_y( double y_ ){
        tprAssert( (y_>=-1.0) && (y_<=1.0) );
        this->y = y_;
        this->consider_threshold_y();
    }

    inline const double &get_x() const {
        return this->x;
    }
    inline const double &get_y() const {
        return this->y;
    }

    //-- 以 double-mpos 为单位 --
    inline const glm::dvec2 to_dmpos() const {
        return glm::dvec2{ this->x, this->y };
    }


    inline bool is_zero() const {
        return ( (this->x==0.0) && (this->y==0.0) );
    }


    //- 目标参数 是否符合 数值范围要求 --
    static bool is_effectVal( double x_, double y_ ){
        bool ret = false;
        if( (x_>=-1.0) && (x_<=1.0) && 
            (y_>=-1.0) && (y_<=1.0) ){
            if( ((x_ >= DirAxes::threshold) || (x_ <= -DirAxes::threshold)) ||
                ((y_ >= DirAxes::threshold) || (y_ <= -DirAxes::threshold)) ){
                ret = true;
            }
        }
        return ret;
    }


    //-- 计算单位向量，并调整 xy值 --
    //   确保 朝任何方向的最大速度 是一致的
    inline void limit_vals(){
        if( this->is_zero() ){
            return;
        }
        glm::dvec2 normal = glm::normalize( glm::dvec2{ this->x, this->y } );
        if( (std::abs(normal.x) < std::abs(this->x)) || (std::abs(normal.y) < std::abs(this->y))  ){
            this->x = normal.x;
            this->y = normal.y;
        }
    }

    //===== static =====//
    static constexpr double threshold = 0.005; //- 阈值，[-0.01, 0.01] 区间的信号不识别

private:

    //-- 将 阈值内的 微小 波动 清除 --
    inline void consider_threshold_x(){
        if( (this->x >= -DirAxes::threshold) && 
            (this->x <=  DirAxes::threshold)  ){
            this->x = 0.0;
        }
    }
    inline void consider_threshold_y(){
        if( (this->y >= -DirAxes::threshold) && 
            (this->y <=  DirAxes::threshold)  ){
            this->y = 0.0;
        }
    }

    //===== vals =====//
    double x {0.0}; //- [-1.0, 1.0]
    double y {0.0}; //- [-1.0, 1.0]
};


#endif 

