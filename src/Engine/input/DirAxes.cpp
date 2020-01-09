/*
 * ======================= DirAxes.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "DirAxes.h"

//-------------------- Engine --------------------//
#include "esrc_coordinate.h"
#include "tprMath.h"

#include "tprDebug.h"


// 将 初始值 修正成 游戏坐标系中的 值
void DirAxes::limit_vals()noexcept{
    if( this->is_zero() ){
        return;
    }
    //--- 确保 朝任何方向的最大速度 是一致的
    glm::dvec2 normal = glm::normalize( this->to_dpos() );
    if( (std::abs(normal.x) < std::abs(this->x)) || (std::abs(normal.y) < std::abs(this->y))  ){
        this->x = normal.x;
        this->y = normal.y;
    }

    //-- 修正 移动方向，从而将玩家输入的方向，对齐于 window 上限制的方向
    //   若不做这道修正，玩家输入的方向，将沿着 world坐标系 去运行
    const auto &worldCoordRef = esrc::get_worldCoordRef();
    glm::dvec2 v = calc_innVec( worldCoordRef.get_rightHand(), this->to_dpos() );

                        //-- 这个计算存在问题，计算出来的值 无法真的 横平竖直地运动，而是存在一个微小的偏转角
                        //   ...

    //--
    this->origin_x = this->x;
    this->origin_y = this->y;
    this->innSet( v.x, v.y );
}




