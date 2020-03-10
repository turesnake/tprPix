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

#include "pch.h"

//-------------------- Engine --------------------//
#include "GoAltiRange.h"
#include "ColliderType.h"
#include "ColliDataFromJson.h"
#include "ID_Manager.h" 


using animActionPosId_t = uint32_t; //- animActionPos id type


//-- 仅用来描述 animFrameSet，所以必须是 静态数据 --//
//   每个 animAction 实例，分配一份
class AnimActionPos{
public:
    AnimActionPos() = default;

    void set_rootAnchorDPosOff( const glm::dvec2 &dposOff_ )noexcept{ this->rootAnchorDPosOff = dposOff_; }

    inline const glm::dvec2 &get_rootAnchorDPosOff() const noexcept{ return this->rootAnchorDPosOff; }

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 animActionPos_id

private:
    glm::dvec2  rootAnchorDPosOff  {};
                        //-- 最原始的数据，从 图元帧左下角ppos，到 rootAnchor点的 fposOff
                        //-- *** 不用对齐于 mapEnt ***
                        //-- 目前仅被用于 ChildMesh 渲染用
};


#endif 

