/*
 * ======================= DirAxes.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "DirAxes.h"

//-------------------- Engine --------------------//
#include "esrc_coordinate.h"


// 将 初始值 修正成 游戏坐标系中的 值
void DirAxes::limit_vals( const glm::dvec2 val_ )noexcept{

    //--- 确保 朝任何方向的最大速度 是一致的
    glm::dvec2 normal = glm::normalize( val_ );

    (std::abs(normal.x) < std::abs(val_.x)) ?
            this->originVal.x = normal.x :
            this->originVal.x = val_.x;
        
    (std::abs(normal.y) < std::abs(val_.y)) ?
            this->originVal.y = normal.y :
            this->originVal.y = val_.y;

    //-- 修正 移动方向，从而将玩家输入的方向，对齐于 window 上限制的方向
    //   若不做这道修正，玩家输入的方向，将沿着 world坐标系 去运行
    const auto &worldCoordRef = esrc::get_worldCoordRef();
    this->worldVal = calc_innVec( worldCoordRef.get_rightHand(), this->originVal );

                        //-- 这个计算存在问题，计算出来的值 无法真的 横平竖直地运动，而是存在一个微小的偏转角
                        //   ...
}




