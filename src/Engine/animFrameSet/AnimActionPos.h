/*
 * ===================== AnimActionPos.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    animFrameSet 中，单张 图元帧 拥有的 全部 pos数据集
 */
#ifndef TPR_ANIM_ACTION_POS_H
#define TPR_ANIM_ACTION_POS_H

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
#include "ColliDataFromJ.h"
#include "ID_Manager.h" 


//-- 从 png文件 读取的 半成品数据 --
class AnimActionSemiData{
public:
    AnimActionSemiData( ColliderType colliderType_ ):
        colliderType(colliderType_)
        {}
    //----- set -----//
    inline void set_lGoAltiRange_onlyOnce( double low_, double high_ )noexcept{
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
    //----- get -----//
    inline ColliderType get_colliderType() const noexcept{ return this->colliderType; }

    inline GoAltiRange      get_lGoAltiRange()const noexcept{ tprAssert(this->isLGoAltiRangeSet); return this->lGoAltiRange; }
    inline const glm::dvec2 &get_rootAnchor()const noexcept{ tprAssert(this->isRootAnchorSet); return this->rootAnchor; }
    inline const glm::dvec2 &get_tailAnchor()const noexcept{ tprAssert(this->isTailAnchorSet); return this->tailAnchor;  }
    inline const glm::dvec2 &get_moveColliRadiusAnchor()const noexcept{ tprAssert(this->isMoveColliRadiusAnchorSet); return this->moveColliRadiusAnchor; }
    inline const glm::dvec2 &get_skillColliRadiusAnchor()const noexcept{ tprAssert(this->isSkillColliRadiusAnchorSet); return this->skillColliRadiusAnchor; }

private:
    ColliderType colliderType {};
    //-- 一律是 左下角到 目标点的 偏移值 --
    glm::dvec2  rootAnchor  {};
    glm::dvec2  tailAnchor  {};
    glm::dvec2  moveColliRadiusAnchor {};
    glm::dvec2  skillColliRadiusAnchor {};
    GoAltiRange   lGoAltiRange {};
    //---- flags ----//
    bool isLGoAltiRangeSet {false};
    bool isRootAnchorSet {false};
    bool isTailAnchorSet  {false};
    bool isMoveColliRadiusAnchorSet  {false};
    bool isSkillColliRadiusAnchorSet {false};
};


using animActionPosId_t = u32_t; //- animActionPos id type



//-- 仅用来描述 animFrameSet，所以必须是 静态数据 --//
//   每个 animAction 实例，分配一份
class AnimActionPos{
public:
    AnimActionPos() = default;

    void init_from_semiData( const AnimActionSemiData &semiData_ );

    //---- get ----//
    inline const glm::dvec2 &get_rootAnchorDPosOff() const noexcept{ return this->rootAnchorDPosOff; }
    inline GoAltiRange get_lGoAltiRange() const noexcept{ return this->lGoAltiRange; } //- return val

    inline const ColliDataFromJ *get_colliDataFromJPtr()const noexcept{ 
        tprAssert( this->colliDataFromJUPtr );
        return this->colliDataFromJUPtr.get();
    }

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 animActionPos_id

private:
    glm::dvec2  rootAnchorDPosOff  {};
                        //-- 最原始的数据，从 图元帧左下角ppos，到 rootAnchor点的 fposOff
                        //-- *** 不用对齐于 mapEnt ***
                        //-- 目前仅被用于 ChildMesh 渲染用

    GoAltiRange   lGoAltiRange {};
                            //- 和 colliEntHead_2 对应，记载每一个 ces实例 携带的 高度区间信息
                            //-- 仅仅是 相对高度区间

    std::unique_ptr<ColliDataFromJ> colliDataFromJUPtr {nullptr};
};


#endif 

