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
#include "tprMath.h"
#include "NineDirection.h"


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
    
    explicit DirAxes( const glm::dvec2 &v_ ):
        x(v_.x),
        y(v_.y)
        {
            tprAssert( (v_.x>=-1.0) && (v_.x<=1.0) &&
                    (v_.y>=-1.0) && (v_.y<=1.0) );
            this->consider_threshold_x();
            this->consider_threshold_y();
        }

    inline void clear_all()noexcept{
        this->x = 0.0;
        this->y = 0.0;
    }

    inline void set( double x_, double y_ )noexcept{
        tprAssert( (x_>=-1.0) && (x_<=1.0) &&
                (y_>=-1.0) && (y_<=1.0) );
        this->x = x_;
        this->y = y_;
        this->consider_threshold_x();
        this->consider_threshold_y();
    }
    inline void set( const glm::dvec2 &v_ )noexcept{
        tprAssert( (v_.x>=-1.0) && (v_.x<=1.0) &&
                (v_.y>=-1.0) && (v_.y<=1.0) );
        this->x = v_.x;
        this->y = v_.y;
        this->consider_threshold_x();
        this->consider_threshold_y();
    }

    //-- 仅用于 InputINS --
    inline void set_x( double x_ )noexcept{
        tprAssert( (x_>=-1.0) && (x_<=1.0) );
        this->x = x_;
        this->consider_threshold_x();
    }
    inline void set_y( double y_ )noexcept{
        tprAssert( (y_>=-1.0) && (y_<=1.0) );
        this->y = y_;
        this->consider_threshold_y();
    }

    inline const double &get_x() const noexcept{ return this->x; }
    inline const double &get_y() const noexcept{ return this->y; }
    inline const glm::dvec2 to_dpos() const noexcept{
        return glm::dvec2{ this->x, this->y };
    }

    inline bool is_zero() const noexcept{
        return ( (this->x==0.0) && (this->y==0.0) );
    }


    //- 目标参数 是否符合 数值范围要求 --
    static bool is_effectVal( double x_, double y_ )noexcept{
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
    inline void limit_vals()noexcept{
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
    inline void consider_threshold_x()noexcept{
        if( is_closeEnough( this->x, 0.0, DirAxes::threshold ) ){
            this->x = 0.0;
        }
    }
    inline void consider_threshold_y()noexcept{
        if( is_closeEnough( this->y, 0.0, DirAxes::threshold ) ){
            this->y = 0.0;
        }
    }

    //===== vals =====//
    double x {0.0}; //- [-1.0, 1.0]
    double y {0.0}; //- [-1.0, 1.0]
};




inline NineDirection dirAxes_2_nineDirection( const DirAxes &da_ ){

    double x = da_.get_x();
    double y = da_.get_y();

    if( da_.is_zero() ){
        return NineDirection::Mid;
    }

    if( x == 0.0 ){
        return (y > 0.0) ?
             NineDirection::Top :
             NineDirection::Bottom;
    }

    if( y == 0.0 ){
        return (x > 0.0) ?
            NineDirection::Right :
            NineDirection::Left;
    }

    double rad = atan2( static_cast<double>(y), static_cast<double>(x) ); // (y,x)
    double eighthPI = 3.1415926 / 8.0;
    
    if( (rad < (-7.0 * eighthPI)) || (rad > (7.0 * eighthPI)) ){ return NineDirection::Left;
    }else if( rad < (-5.0 * eighthPI) ){ return NineDirection::LeftBottom;
    }else if( rad < (-3.0 * eighthPI) ){ return NineDirection::Bottom;
    }else if( rad < (-1.0 * eighthPI) ){ return NineDirection::RightBottom;
    }else if( rad < (1.0 * eighthPI) ){ return NineDirection::Right;
    }else if( rad < (3.0 * eighthPI) ){ return NineDirection::RightTop;
    }else if( rad < (5.0 * eighthPI) ){ return NineDirection::Top;
    }else{                              return NineDirection::LeftTop;
    }
}




#endif 

