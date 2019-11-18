/*
 * ====================== ColliderType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLLIDER_TYPE_H
#define TPR_COLLIDER_TYPE_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>

//------------------- Engine --------------------//
#include "tprAssert.h"

enum class ColliderType{
    Nil,      //- not call move collide detect

    Circular,
    Arc,      //- only support for skillCollide
    Square,
};

inline ColliderType str_2_ColliderType( const std::string &name_ )noexcept{
    if( name_ == std::string{"Nil"} ){              return ColliderType::Nil;
    }else if( name_ == std::string{"Circular"} ){   return ColliderType::Circular;
    }else if( name_ == std::string{"Arc"} ){        return ColliderType::Arc;
    }else if( name_ == std::string{"Square"} ){     return ColliderType::Square;
    }else{
        tprAssert(0);
        return ColliderType::Circular; //- never reach
    }
}


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
    Square( const glm::dvec2 &dpos_):
        dpos(dpos_),
        radius( Square::unifiedRadius )
        {}
    
    //----- vals -----//
    glm::dvec2  dpos       {};
    double      radius     {}; // 未来可以被取消 ...
    //----- static -----//
    // 暂时只有一个 半径值，所有实例 都使用此值
    static double unifiedRadius;
};


#endif

