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
    virtual Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_, bool is_for_dogo_ ) const = 0;
    //--- only support in square ---
    virtual Square calc_square( const glm::dvec2 &goCurrentDPos_ ) const = 0;
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
        return ColliDataFromJ_Nil::emptyVec;
    }
    inline Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_, bool is_for_dogo_ )const override{ //- not exist
        tprAssert(0); 
        return Circular{};
    }
    
    inline Square calc_square( const glm::dvec2 &goCurrentDPos_ )const override{  //- not exist
        tprAssert(0); 
        return Square{};
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变

    //===== static =====//
    static glm::dvec2              emptyDvec2;
    static std::vector<glm::dvec2> emptyVec;
};




class ColliDataFromJ_Circular : public ColliDataFromJ{
public:
    ColliDataFromJ_Circular()
        {
            if( !ColliDataFromJ_Circular::isStaticInit ){
                ColliDataFromJ_Circular::isStaticInit = true;
                ColliDataFromJ_Circular::init_for_static();
            }
        }

    inline void makeSure_colliPointDPosOffs_isNotEmpty()const noexcept{
        tprAssert( !this->colliPointDPosOffs.empty() );
    }


    inline Circular calc_circular(const glm::dvec2 &goCurrentDPos_, CollideFamily family_, bool is_for_dogo_ )const override{
        double radius {};
        if( family_ == CollideFamily::Move ){
            radius = is_for_dogo_ ? 
                        ColliDataFromJ_Circular::moveColliRadius_for_dogo :
                        this->moveColliRadius;
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
    inline Square calc_square( const glm::dvec2 &goCurrentDPos_ )const override{  //- not exist
        tprAssert(0); 
        return Square{};
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变
    double  moveColliRadius {};
    double  skillColliRadius {};
    std::vector<glm::dvec2> colliPointDPosOffs {};//- 移动碰撞检测点，会被自动生成

    //===== static =====//
    static double      moveColliRadius_for_dogo; // 统一半径：24.0
    static glm::dvec2  emptyDvec2;
    static bool        isStaticInit;
    static std::vector<glm::dvec2> colliPointDPosOffs_for_dogo;//- 所有 dogo 使用统一半径的 碰撞体
private:
    static void init_for_static()noexcept;
};


class ColliDataFromJ_Square : public ColliDataFromJ{
public:
    ColliDataFromJ_Square()
        {
            if( !ColliDataFromJ_Square::isStaticInit ){
                ColliDataFromJ_Square::isStaticInit = true;
                ColliDataFromJ_Square::init_for_static(); // only once
            }
        }

    //----- get -----//
    inline ColliderType get_colliderType()const override{ return this->colliderType; }
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const override{ return ColliDataFromJ_Square::colliPointDPosOffs; }

    inline Square calc_square( const glm::dvec2 &goCurrentDPos_ )const override{ 
        return Square{ goCurrentDPos_ };
    }

    //---- not exist funcs ----//
    inline double get_moveColliRadius() const override{  //- not exist
        tprAssert(0); 
        return 0.0; 
    }
    inline double get_skillColliRadius() const override{  //- not exist
        tprAssert(0); 
        return 0.0; 
    }
    inline Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_, bool is_for_dogo_ )const override{ //- not exist
        tprAssert(0); 
        return Circular{};
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变

    //===== static =====//
    static glm::dvec2               emptyDvec2;
    static bool                     isStaticInit;
    static std::vector<glm::dvec2>  colliPointDPosOffs;//- 移动碰撞检测点，只有一套，所有实例通用
    
private:
    static void init_for_static()noexcept;
};



#endif 

