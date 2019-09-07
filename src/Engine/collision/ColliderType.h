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
    Capsule,
};

inline ColliderType str_2_ColliderType( const std::string &name_ )noexcept{
    if( name_ == std::string{"Nil"} ){
        return ColliderType::Nil;
    }
    if( name_ == std::string{"Circular"} ){
        return ColliderType::Circular;
    }else if( name_ == std::string{"Capsule"} ){
        return ColliderType::Capsule;
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

    inline Circular calc_new_circular( const glm::dvec2 &dposOff_ ) const noexcept{
        return Circular{ this->dpos+dposOff_, this->radius };    
    }

    glm::dvec2  dpos   {};
    double      radius {};
};


class Capsule{
public:
    Capsule() = default;

    inline Capsule calc_new_capsule( const glm::dvec2 &dposOff_ ) const noexcept{
        return Capsule{ this->dpos + dposOff_,
                        this->root_2_tail,
                        this->longLen,
                        this->radius };
    }

    glm::dvec2  dpos        {};
    glm::dvec2  root_2_tail {};
    double      longLen     {};
    double      radius      {};
};



#endif

