/*
 * ===================== ColliDataFromJson.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   从 Jpng 读取的 colli 数据
 */
#ifndef TPR_COLLI_DATA_FROM_JPNG_H
#define TPR_COLLI_DATA_FROM_JPNG_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "GoAltiRange.h"
#include "ColliderType.h"
#include "SignInMapEnts_Square_Type.h"


// 可能要改名为 FromJson 

class ColliDataFromJson{
public:
    ColliDataFromJson()=default;
    virtual ~ColliDataFromJson()=default;
    //---
    virtual ColliderType get_colliderType()const =0;
    virtual double get_moveColliRadius()const =0;
    virtual double get_skillColliRadius()const =0;
    virtual const std::vector<glm::dvec2> &get_colliPointDPosOffs()const =0;
    //--- only support in circular ---
    virtual Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_ ) const = 0;
    //--- only support in square ---
    virtual Square calc_square( const glm::dvec2 &goCurrentDPos_ )const = 0;
    virtual SignInMapEnts_Square_Type get_signInMapEnts_square_type()const = 0;
};



class ColliDataFromJson_Nil : public ColliDataFromJson{
public:
    ColliDataFromJson_Nil():
        colliderType(ColliderType::Nil)
        {}

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
        return ColliDataFromJson_Nil::emptyVec;
    }
    inline Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{ //- not exist
        tprAssert(0); 
        return Circular{};
    }
    
    inline Square calc_square( const glm::dvec2 &goCurrentDPos_ )const override{  //- not exist
        tprAssert(0); 
        return Square{};
    }

    inline SignInMapEnts_Square_Type get_signInMapEnts_square_type()const override{
        tprAssert(0);
        return SignInMapEnts_Square_Type::T_1m1;
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变

    //===== static =====//
    static std::vector<glm::dvec2> emptyVec;
    static std::unique_ptr<ColliDataFromJson> nillInstance; 
                            // 一些类似 groundGo 的go，无法通过标准流程被组件，但也需要一个 Nil类型的 ColliDataFromJson 数据
                            // 此处提供给它们一个 通用的 实例
};




class ColliDataFromJson_Circular : public ColliDataFromJson{
public:
    ColliDataFromJson_Circular():
        colliderType(ColliderType::Circular)
        {}

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
    inline Square calc_square( const glm::dvec2 &goCurrentDPos_ )const override{  //- not exist
        tprAssert(0); 
        return Square{};
    }

    inline SignInMapEnts_Square_Type get_signInMapEnts_square_type()const override{
        tprAssert(0);
        return SignInMapEnts_Square_Type::T_1m1;
    }

    //----- vals -----//
    ColliderType  colliderType {}; //- nil/cir/cap  一经初始，用不改变
    double  moveColliRadius {};
    double  skillColliRadius {};
    std::vector<glm::dvec2> colliPointDPosOffs {};//- 移动碰撞检测点，会被自动生成

    //===== static =====//
};


class ColliDataFromJson_Square : public ColliDataFromJson{
public:
    ColliDataFromJson_Square():
        colliderType(ColliderType::Square)
        {}

    //----- get -----//
    inline ColliderType get_colliderType()const override{ return this->colliderType; }
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const override{ return ColliDataFromJson_Square::colliPointDPosOffs; }

    inline Square calc_square( const glm::dvec2 &goCurrentDPos_ )const override{ 
        return Square{ goCurrentDPos_ };
    }

    inline SignInMapEnts_Square_Type get_signInMapEnts_square_type()const override{
        return this->signInMapEnts_square_type;
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
    inline Circular calc_circular( const glm::dvec2 &goCurrentDPos_, CollideFamily family_ )const override{ //- not exist
        tprAssert(0); 
        return Circular{};
    }

    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 

    //----- vals -----//
    ColliderType                colliderType {}; //- nil/cir/cap  一经初始，用不改变
    SignInMapEnts_Square_Type   signInMapEnts_square_type {};


    //===== static =====//
    static std::vector<glm::dvec2>  colliPointDPosOffs;//- 移动碰撞检测点，只有一套，所有实例通用    
};



#endif 

