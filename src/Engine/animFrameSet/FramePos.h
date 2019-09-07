/*
 * ========================= FramePos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    animFrameSet 中，单张 图元帧 拥有的 全部 pos数据集
 */
#ifndef TPR_FRAME_POS_2_H
#define TPR_FRAME_POS_2_H

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


class JData_InCircular{
public:
    JData_InCircular() =default;

    //----- get -----//
    inline double get_moveColliRadius() const noexcept{ return this->moveColliRadius; };
    inline double get_skillColliRadius() const noexcept{ return this->skillColliRadius; };
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const noexcept{ return this->colliPointDPosOffs; }
    
    //----- vals -----//
    double  moveColliRadius {};
    double  skillColliRadius {};
    std::vector<glm::dvec2> colliPointDPosOffs {};//- 从 rootAnchor 到各个 colliPoint 的 偏移值 dpos
                                                //- include rootAnchor and tailAnchor
};


class JData_InCapsule{
public:
    JData_InCapsule() = default;

    //----- get -----//
    inline double get_moveColliRadius()  const noexcept{ return this->moveColliRadius; };
    inline double get_skillColliRadius() const noexcept{ return this->skillColliRadius; };
    inline double get_longLen()          const noexcept{ return this->longLen; };
    inline const std::vector<glm::dvec2> &get_colliPointDPosOffs() const noexcept{ return this->colliPointDPosOffs; }
    inline const glm::dvec2 &get_rootAnchor_2_tailAnchor() const noexcept{ return this->rootAnchor_2_tailAnchor; };

    //----- vals -----//
    glm::dvec2  rootAnchor_2_tailAnchor {}; //- only in capsule mode
    double  moveColliRadius {};
    double  skillColliRadius {}; // eque to moveColliRadius
    double  longLen          {}; // 两 锚点间 距离
    std::vector<glm::dvec2> colliPointDPosOffs {};//- 从 rootAnchor 到各个 colliPoint 的 偏移值 dpos
                                                //- include rootAnchor and tailAnchor
};


//-- 从 png文件 读取的 半成品数据 --
class FramePosSemiData{
public:
    FramePosSemiData( const ColliderType &colliderType_ ):
        colliderType(colliderType_)
        {}
    //----- set -----//
    inline void set_lGoAltiRange_onlyOnce( char low_, char high_ )noexcept{
        tprAssert( !this->isLGoAltiRangeSet );
        this->isLGoAltiRangeSet = true;
        this->lGoAltiRange.set( low_, high_ );
    }
    inline void set_rootAnchor_onlyOnce( const glm::dvec2 &v_ )noexcept{ 
        tprAssert( !this->isRootAnchorSet );
        this->isRootAnchorSet = true;
        this->rootAnchor = v_; 
    }
    inline void set_tailAnchor_onlyOnce(  const glm::dvec2 &v_ )noexcept{ 
        tprAssert( !this->isTailAnchorSet );
        this->isTailAnchorSet = true;
        this->tailAnchor = v_; 
    }
    inline void set_moveColliRadiusAnchor_onlyOnce( const glm::dvec2 &v_ )noexcept{
        tprAssert( !this->isMoveColliRadiusAnchorSet );
        this->isMoveColliRadiusAnchorSet = true; 
        this->moveColliRadiusAnchor = v_; 
    }
    inline void set_skillColliRadiusAnchor_onlyOnce( const glm::dvec2 &v_ )noexcept{
        tprAssert( !this->isSkillColliRadiusAnchorSet );
        this->isSkillColliRadiusAnchorSet = true; 
        this->skillColliRadiusAnchor = v_; 
    }
    inline void pushback_2_colliPoints( const glm::dvec2 &v_ )noexcept{
        this->colliPoints.push_back( v_ );
    }
    //----- get -----//
    inline const ColliderType &get_colliderType() const noexcept{ return this->colliderType; };

    inline const GoAltiRange &get_lGoAltiRange() const noexcept{ tprAssert(this->isLGoAltiRangeSet); return this->lGoAltiRange; };
    inline const glm::dvec2 &get_rootAnchor()    const noexcept{ tprAssert(this->isRootAnchorSet); return this->rootAnchor; };
    inline const glm::dvec2 &get_tailAnchor()     const noexcept{ tprAssert(this->isTailAnchorSet); return this->tailAnchor;  };
    inline const glm::dvec2 &get_moveColliRadiusAnchor()  const noexcept{ tprAssert(this->isMoveColliRadiusAnchorSet); return this->moveColliRadiusAnchor; };
    inline const glm::dvec2 &get_skillColliRadiusAnchor() const noexcept{ tprAssert(this->isSkillColliRadiusAnchorSet); return this->skillColliRadiusAnchor; };
    
    inline const std::vector<glm::dvec2> &get_colliPointsRef() const noexcept{ tprAssert(!this->colliPoints.empty()); return this->colliPoints; };

private:
    ColliderType colliderType {};
    //-- 一律是 左下角到 目标点的 偏移值 --
    glm::dvec2  rootAnchor  {};
    glm::dvec2  tailAnchor  {};
    glm::dvec2  moveColliRadiusAnchor {};
    glm::dvec2  skillColliRadiusAnchor {};
    std::vector<glm::dvec2> colliPoints {}; // not include root/tail Anchor yet
    GoAltiRange   lGoAltiRange {};
    //---- flags ----//
    bool isLGoAltiRangeSet {false};
    bool isRootAnchorSet {false};
    bool isTailAnchorSet  {false};
    bool isMoveColliRadiusAnchorSet  {false};
    bool isSkillColliRadiusAnchorSet {false};
};


//-- 仅用来描述 animFrameSet，所以必须是 静态数据 --//
class FramePos{
    using F_double     = std::function<double()>;
    using F_c_dvec2Ref = std::function<const glm::dvec2 &()>;
    using F_colliPointRef = std::function<const std::vector<glm::dvec2> &()>;

public:
    FramePos() = default;

    void init_from_semiData( const FramePosSemiData &semiData_ );

    //---- get ----//
    inline const glm::dvec2 &get_rootAnchorDPosOff() const noexcept{ return this->rootAnchorDPosOff; }
    inline GoAltiRange get_lGoAltiRange() const noexcept{ return this->lGoAltiRange; } //- return val
    inline bool get_isCarryAffect() const noexcept{ return this->isCarryAffect; }
    inline const ColliderType &get_colliderType() const noexcept{ return this->colliderType; }

    inline double get_longLen() const noexcept{ //- only in capsule
        tprAssert(this->capsuleUPtr);
        return this->capsuleUPtr->get_longLen(); 
    }


    inline Circular calc_circular(  const glm::dvec2 &goCurrentDPos_,
                                    const CollideFamily &family_ ) const noexcept{
        tprAssert( this->circularUPtr );
        double radius {};
        if( family_ == CollideFamily::Move ){
            radius = this->circularUPtr->get_moveColliRadius();
        }else{
            radius = this->circularUPtr->get_skillColliRadius();
        }
        return Circular{ goCurrentDPos_, radius };
    }

    inline Capsule calc_capsule(const glm::dvec2 &goCurrentDPos_,
                                const CollideFamily &family_ ) const noexcept{
        tprAssert( this->capsuleUPtr );
        double radius {};
        if( family_ == CollideFamily::Move ){
            radius = this->capsuleUPtr->get_moveColliRadius();
        }else{
            radius = this->capsuleUPtr->get_skillColliRadius();
        }
        return Capsule{ goCurrentDPos_, 
                        this->capsuleUPtr->get_rootAnchor_2_tailAnchor(),
                        this->capsuleUPtr->get_longLen(),
                        radius };
    }


    //--- functors ---//
    F_c_dvec2Ref    get_rootAnchor_2_tailAnchor {nullptr};
    F_double        get_moveColliRadius        {nullptr};
    F_double        get_skillColliRadius       {nullptr};
    F_colliPointRef get_colliPointDPosOffsRef  {nullptr};

    
private:
    ColliderType  colliderType {};  // Nil / Circular / Capsule

    glm::dvec2  rootAnchorDPosOff  {};
                        //-- 最原始的数据，从 图元帧左下角ppos，到 rootAnchor点的 fposOff
                        //-- *** 不用对齐于 mapEnt ***
                        //-- 目前仅被用于 ChildMesh 渲染用

    //- only one will be used, or none.
    std::unique_ptr<JData_InCircular> circularUPtr {nullptr};
    std::unique_ptr<JData_InCapsule>  capsuleUPtr  {nullptr};


    GoAltiRange   lGoAltiRange {};
                            //- 和 colliEntHead_2 对应，记载每一个 ces实例 携带的 高度区间信息
                            //-- 仅仅是 相对高度区间 

    bool        isCarryAffect {false}; 
                            //- 本 ceh 是否携带 affect／碰撞触发的主动技能
                            // 挥刀动作中，刀部的 ceh，火球的ceh，都会携带 affect
                            // 常规生物的 身体部分的 ceh，则不会携带 affect
                            // 用于 碰撞检测
                            //...
                            // 这个概念可能要被修改...
                            //
                            // 第三版中，没被任何代码设置，维持初始值...

    //bool        isBody       {true};
                            //- body    - “实体ceh” 会被登记到 mapent上，比如 生物的身体部分。
                            //- virtual - “虚幻ceh” 不会被登记到mapent上，比如火球，刀。
                            // ------------------
                            //  目前，这个值 主要在 png文件 内被设置
                            //  在 J.png 数据上， 
                            //  (low, high)         时，isBody == true
                            //  (100+low, 100+high) 时，isBody == false
                            // --------------
                            // 在未来，需要提供 代码层的 改写口，方便在代码中制作一些 隐形的 技能向 碰撞区
                            //
                            // 第三版中，没被任何代码设置，维持初始值...

};


#endif 

