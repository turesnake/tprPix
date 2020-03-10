/*
 * ====================== ColliderType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLLIDER_TYPE_H
#define TPR_COLLIDER_TYPE_H

#include "pch.h"

enum class ColliderType{
    Nil,
    //---
    Circular,   // 最全最复杂的 碰撞体，所有 活体go。只能用此类型
                // 参与 moveCollide, 也参与 skillCollide
                // 唯一可以 移动 的类型，
                // 拥有自定义 碰撞圆半径，以便别的go，碰到它
                // 当扮演 dogo 时，则使用一个统一的 标准半径值
                // 每次移动，都需要及时更新自己的 mapent 登记信息 

    Square,     // 专用于 静态地景go。
                // 参与 moveCollide, 不参与 skillCollide
                // 无法移动，碰撞区就是一个 完整的 mapent

    Arc,        //  参与 skillCollide，
                //  可以移动，但不参与 moveCollide
                //  不会被登记到 mapent 上


};


ColliderType str_2_colliderType( const std::string &name_ )noexcept;


enum class CollideState{
    Separate,  //- 相离
    Adjacent,  //- 相临 （会出发滑动效果）
    Intersect, //- 相交
};


enum class CollideFamily{
    Move,
    Skill,
};



class Circular{
public:
    Circular() = default;

    Circular( const glm::dvec2 &dpos_, double radius_ ):
        dpos(dpos_),
        radius(radius_)
        {}

    inline Circular calc_new_circular( const glm::dvec2 &dposOff_ ) const noexcept{
        return Circular{ this->dpos+dposOff_, this->radius };    
    }
    //----- vals -----//
    glm::dvec2  dpos   {};
    double      radius {};
    //----- static -----//
    static double radius_for_dogo; // 扮演 dogo 时，使用一个标准值 24.0 
};




class ArcLine{
public:
    ArcLine()=default;

    ArcLine(const glm::dvec2    &dpos_,
            const glm::dvec2    &forward_,
            double              radius_,
            double              halfRadian_):
        dpos(dpos_),
        forward(forward_),
        radius(radius_),
        halfRadian(halfRadian_)
        {}

    //----- vals -----//
    glm::dvec2  dpos       {};
    glm::dvec2  forward    {};
    double      radius     {};
    double      halfRadian {};// 半弧   
};



class Square{
public:
    Square()=default;
    explicit Square( const glm::dvec2 &dpos_):
        dpos(dpos_),
        radius( Square::unifiedRadius )
        {}
    
    //----- vals -----//
    glm::dvec2  dpos       {};
    double      radius     {}; // 未来可以被取消 ...
    //----- static -----//
    static double unifiedRadius; // 唯一且统一的半径值
};


#endif

