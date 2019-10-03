/*
 * ===================== ColliDataFromJ.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   从 Jpng 读取的 colli 数据
 */
#ifndef TPR_COLLI_DATA_FROM_J_H
#define TPR_COLLI_DATA_FROM_J_H


//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <memory>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GoAltiRange.h"
#include "IntVec.h"
#include "ColliderType.h"



class ColliDataFromJ{
public:
    ColliDataFromJ()=default;
    virtual ~ColliDataFromJ()=default;
    //---
    virtual ColliderType get_colliderType()const =0;
    virtual double get_moveColliRadius()const =0;
    virtual double get_skillColliRadius()const =0;
    virtual const std::vector<glm::dvec2> &get_colliPointDPosOffs()const =0;
    //--- only support in circular ---
    virtual Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_ ) const = 0;
    //--- only support in capsule ---
    virtual double get_longLen()const =0;
    virtual const glm::dvec2 &get_rootAnchor_2_tailAnchor()const =0;
    virtual Capsule calc_capsule(   const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const =0;
};



class ColliDataFromJ_Nil : public ColliDataFromJ{
public:
    ColliDataFromJ_Nil() =default;

    //----- get -----//
    inline ColliderType get_colliderType()const override{ return this->colliderType; }
    //---- not exist funcs ----//
    inline double get_moveColliRadius() const override{  //- not exist
        tprAssert(0); 
        return 0.0; 
    }
    inline double get_skillColliRadius() const override{  //- not exist
        tprAssert(0); 
        return 0.0; 
    }
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const override{  //- not exist
        tprAssert(0); 
        return this->emptyVec;
    }
    inline double get_longLen()const override{  //- not exist
        tprAssert(0);
        return 0.0;
    }
    inline const glm::dvec2 &get_rootAnchor_2_tailAnchor() const override{  //- not exist
        tprAssert(0); 
        return this->emptyDvec2;
    }
    inline Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{ //- not exist
        tprAssert(0); 
        return Circular{};
    }
    inline Capsule calc_capsule(const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{  //- not exist
        tprAssert(0); 
        return Capsule{};
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变
    //---
    glm::dvec2              emptyDvec2 {};
    std::vector<glm::dvec2> emptyVec {};
};




class ColliDataFromJ_Circular : public ColliDataFromJ{
public:
    ColliDataFromJ_Circular() =default;

    inline void makeSure_colliPointDPosOffs_isNotEmpty()const noexcept{
        tprAssert( !this->colliPointDPosOffs.empty() );
    }


    inline Circular calc_circular(const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{
        double radius {};
        if( family_ == CollideFamily::Move ){
            radius = this->moveColliRadius;
        }else{
            radius = this->skillColliRadius;
        }
        return Circular{ goCurrentDPos_, radius };
    }

    //----- get virtual -----//
    inline ColliderType get_colliderType()const override{ return this->colliderType; }
    inline double get_moveColliRadius() const override{ return this->moveColliRadius; }
    inline double get_skillColliRadius() const override{ return this->skillColliRadius; }
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const override{ return this->colliPointDPosOffs; }

    //---- not exist funcs ----//
    inline double get_longLen()const override{  //- not exist
        tprAssert(0); 
        return 0.0;
    }
    inline const glm::dvec2 &get_rootAnchor_2_tailAnchor() const override{ //- not exist
        tprAssert(0); 
        return this->emptyDvec2;
    }
    inline Capsule calc_capsule(const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{ //- not exist
        tprAssert(0); 
        return Capsule{};
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变
    double  moveColliRadius {};
    double  skillColliRadius {};
    std::vector<glm::dvec2> colliPointDPosOffs {};//- 移动碰撞检测点，会被自动生成
    //---
    glm::dvec2  emptyDvec2 {};
};


class ColliDataFromJ_Capsule : public ColliDataFromJ{
public:
    ColliDataFromJ_Capsule() = default;

    inline void makeSure_colliPointDPosOffs_isNotEmpty()const noexcept{
        tprAssert( !this->colliPointDPosOffs.empty() );
    }


    inline Capsule calc_capsule(const glm::dvec2 &goCurrentDPos_, CollideFamily family_)const override{
        double radius {};
        if( family_ == CollideFamily::Move ){
            radius = this->moveColliRadius;
        }else{
            radius = this->skillColliRadius;
        }
        return Capsule{ goCurrentDPos_, 
                        this->rootAnchor_2_tailAnchor,
                        this->longLen,
                        radius };
    }

    //----- get virtual -----//
    inline ColliderType get_colliderType()const override{ return this->colliderType; }
    inline double get_moveColliRadius()  const override{ return this->moveColliRadius; }
    inline double get_skillColliRadius() const override{ return this->skillColliRadius; }
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const override{ return this->colliPointDPosOffs; }
    //----
    inline double get_longLen()const override{ return this->longLen; }
    inline const glm::dvec2 &get_rootAnchor_2_tailAnchor() const override{ return this->rootAnchor_2_tailAnchor; }

    //---- not exist funcs ----//
    inline Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{ //- not exist
        tprAssert(0); 
        return Circular{};
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变
    glm::dvec2  rootAnchor_2_tailAnchor {}; //- only in capsule mode
    double  moveColliRadius {};
    double  skillColliRadius {}; // eque to moveColliRadius
    double  longLen          {}; // 两 锚点间 距离
    std::vector<glm::dvec2> colliPointDPosOffs {};//- 移动碰撞检测点，会被自动生成
};

#endif 

