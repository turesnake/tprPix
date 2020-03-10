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

//-------------------- CPP --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprMath.h"
#include "NineDirection.h"


// 游戏世界 和 程序窗口， 存在 两套坐标系: world, origin
// 程序窗口 坐标系，是符合常规视觉的，直上直下的 坐标系，也对应 UI坐标系，对应 input坐标系
// 游戏世界 坐标系，则旋转了 30度
class DirAxes{
public:
    DirAxes( double x_=0.0, double y_=0.0 )
        {
            this->innSet( x_, y_ );
        }
    
    explicit DirAxes( const glm::dvec2 &v_ )
        {
            this->innSet( v_.x, v_.y );
        }


    //inline void set( double x_, double y_ )noexcept{ this->innSet( x_, y_ ); }
    //inline void set( const glm::dvec2 &v_ )noexcept{ this->innSet( v_.x, v_.y ); }

    inline void clear_all()noexcept{
        this->originVal = glm::dvec2{0.0, 0.0};
        this->worldVal = glm::dvec2{0.0, 0.0};
    }


    inline const glm::dvec2 &get_worldVal()const noexcept{ return this->worldVal; }
    inline const glm::dvec2 &get_originVal()const noexcept{ return this->originVal; }


    // 0 代表 无方向。此时， worldVal 和 originVal 都会是 0，不矛盾
    inline bool is_zero() const noexcept{
        return is_closeEnough( this->worldVal, glm::dvec2{0.0, 0.0}, DirAxes::threshold );
    }

    //===== static =====//
    static constexpr double threshold = 0.005; //- 阈值，[-0.01, 0.01] 区间的信号不识别
private:
    
    inline void innSet( double x_, double y_ )noexcept{
        //----------------//
        // -1- 确保参数范围合法
        //----------------//
        tprAssert(  (x_>=-1.0) && (x_<=1.0) &&
                    (y_>=-1.0) && (y_<=1.0) );

        glm::dvec2 tmp { x_, y_ };

        //----------------//
        // -2- 检测死区
        //----------------//
        bool is_x_zero {false};
        bool is_y_zero {false};

        if( is_closeEnough<double>( tmp.x, 0.0, DirAxes::threshold ) ){
            tmp.x = 0.0;
            is_x_zero = true;
        }
        if( is_closeEnough<double>( tmp.y, 0.0, DirAxes::threshold ) ){
            tmp.y = 0.0;
            is_y_zero = true;
        }

        //----------------//
        // -3- 转换为标准向量 存入  originVal
        // -4- 旋转坐标系，  值存入 worldVal
        //----------------//
        if( !is_x_zero || !is_y_zero ){
            this->limit_vals( tmp );
        }else{
            this->originVal = glm::dvec2{ 0.0, 0.0 };
            this->worldVal = glm::dvec2{ 0.0, 0.0 };
        }
    }

    void limit_vals( const glm::dvec2 val_ )noexcept;


    //===== vals =====//
    // Input坐标系 / UI坐标系 / 程序窗口坐标系
    glm::dvec2 originVal {};


    // 游戏世界 坐标系
    //---
    // 假设，originVal = {0,1}, 指向 正上方
    // worldVal 会是 偏转后的，指向右上方的值，
    // 这个值放进 游戏世界坐标系中，正式呈现为 视觉上的 正上方
    glm::dvec2 worldVal {};

};



/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 * 这个实现，没有检测 origin 数据
 */
inline bool operator == ( DirAxes a_, DirAxes b_ ) noexcept {
    return is_closeEnough( a_.get_worldVal(), b_.get_worldVal(), DirAxes::threshold );
}
inline bool operator != ( DirAxes a_, DirAxes b_ ) noexcept {
    return !(a_==b_);
}



inline NineDirection dirAxes_2_nineDirection( const DirAxes &da_ )noexcept{

    // 使用 符合 Input坐标系 的值，符合玩家视觉
    // 现在，玩家输入的 left 方向，就能让 go 显示 left 方向，同时向 window left 方向移动
    // 而不是 worldCoord 坐标系中的 left 方向
    double x = da_.get_originVal().x;
    double y = da_.get_originVal().y;


    if( da_.is_zero() ){
        return NineDirection::Center;
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

