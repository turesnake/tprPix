/*
 * ======================= Collision.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLLISION_2_H
#define TPR_COLLISION_2_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <utility> //pair
#include <variant>

//-------------------- Engine --------------------//
#include "ColliderType.h"
#include "GameObjPos.h"
#include "GameObjType.h"
#include "SignInMapEnts_Circle.h"
#include "NineDirection.h"
#include "SignInMapEnts_Square_Type.h"

#include "mapEntKey.h"


//--- need ---//
class GameObj;


//-- 存在两种碰撞检测:
// --1-- 移动碰撞检测
// --2-- 技能碰撞检测
// 这两种 相互独立。并行存在
//   这意味着，go并不需要停下来才能 释放技能。 
//   当然，由于 动画制作的问题，大部分技能在施展期间，还是会强制停下 go 的移动
class Collision{
    using F_get_colliPointDPosOffsRef = std::function<const std::vector<glm::dvec2> &()>;
    using F_is_extraPassableDogoSpeciesId = std::function<bool(goSpeciesId_t)>; // param: dogoSpeciesId
public:
    
    explicit Collision( GameObj &goRef_ ):
        goRef(goRef_)
        {}

    glm::dvec2 detect_moveCollide( const glm::dvec2 &moveVec_ ); // tmp


    //-- only call in go init --
    inline void init_signInMapEnts_circle( const glm::dvec2 &newGoDPos_, F_get_colliPointDPosOffsRef func_1_ )noexcept{  
        this->signInMapEnts_cir_uptr = std::make_unique<SignInMapEnts_Circle>( newGoDPos_, func_1_ );
    }

    inline void set_isDoPass( bool val_ )noexcept{ this->isDoPass = val_; }
    inline void set_isBePass( bool val_ )noexcept{ this->isBePass = val_; }
    inline bool get_isDoPass()const noexcept{ return this->isDoPass; }

    // bego pass 检测其实存在 2步：
    // isBePass == true: 允许一切 dogo 穿过
    // isBePass == false: 若 dogo 为少数登记类型，也可以穿过
    //                      剩余的 dogo 才会被禁止穿过
    inline bool get_isBePass( goSpeciesId_t dogoSpeciesId_ )const noexcept{ 
        if( this->isBePass ){
            return true;
        }
        return (this->Is_extraPassableDogoSpeciesId != nullptr) ?
                    this->Is_extraPassableDogoSpeciesId(dogoSpeciesId_) :
                    false;
    }

    inline void set_functor_Is_extraPassableDogoSpeciesId( F_is_extraPassableDogoSpeciesId functor_ )noexcept{
        this->Is_extraPassableDogoSpeciesId = functor_;
    }

    inline const std::set<IntVec2> &get_current_signINMapEnts_circle_ref()const noexcept{
        return this->signInMapEnts_cir_uptr->get_currentSignINMapEntsRef();
    }    

    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 

private:
    std::pair<bool, glm::dvec2> collect_AdjacentBegos_and_deflect_moveVec( const glm::dvec2 &moveVec_ );
    std::pair<bool, glm::dvec2> collect_IntersectBegos_and_truncate_moveVec( const glm::dvec2 &moveVec_ );

    void reSignUp_dogo_to_chunk_and_mapents( const glm::dvec2 &moveVec_ )noexcept;


    //======== vals ========//
    GameObj    &goRef;

    // 仅被 cir-dogo 使用 
    std::unique_ptr<SignInMapEnts_Circle> signInMapEnts_cir_uptr {nullptr};

    
    //======== flags ========//
    bool  isDoPass  {false};//- 当自己为 主动go 时，是否穿过 被动go；
                            //  如果本go 是 “穿透箭”。可将此值设置为 true
                            //  此时，本go 将无视 被动go 的状态，移动穿过一切 被动go
    bool  isBePass  {false};//- 当自己为 被动go 时，是否允许 主动go 穿过自己；
                            //  如果本go 是 “草”／“腐蚀液”，可将此值 设置为 true
                            //  此时，本go 将允许任何 go 从自己身上 穿过

    F_is_extraPassableDogoSpeciesId Is_extraPassableDogoSpeciesId {nullptr};
                            // 当 bego.isBePass == false 时，
                            // 将额外允许一组特殊的 dogo 穿过自己
                            // 比如，只有 “鸡”，可以穿过 “鸡笼”

                // 推荐用法：
                //-- 对于 草这种允许被dogo穿过，自己又无法移动的。 isBePass=true， isDoPass 随意（无效）
                //-- 对于 常规生物，2值都设置为 false
                //-- 对于 允许别人穿过自己，但自己无法穿透任意墙壁的 dogo，类似 mc中的羊，isDoPass=false, isBePass=true
                //    一些小动物，npc 特别适合这种
                //-- 对于 带穿透属性的 箭矢，isDoPass=true， isBePass 随意（无效）


    //===== static =====//
    static void build_a_scanBody(   const glm::dvec2 &moveVec_,
                                    const glm::dvec2 &dogoDPos_ );


    static std::vector<glm::dvec2> obstructNormalVecs;
                            // moveCollide 第一阶段，在 dogo起始位置，所有与之 相邻 的bego，
                            // 对 dogo 施与的 阻挡法向量集（墙壁法向量集）
                            // 用来修正 moveVec

    static std::set<NineDirection> confirmedAdjacentMapEnts;
                            // 确认会发生 碰撞的 相邻 mapents [0,2]

    static std::unordered_map<goid_t, glm::dvec2> adjacentCirBeGos; // 相邻关系的 cir_begos
                            // --1-- begoid
                            // --2-- obstructNormalVec
                            // 相邻go 是触发 ”滑动式位移“ 的唯一途径

    static std::unordered_set<goid_t> begoids_circular; 
    static std::unordered_set<goid_t> begoids; 
                            //- 从 signInMapEnts 中收集的 半有效 begoids

    static std::vector<IntVec2> mapEnts_in_scanBody;
                            // 用于 squ_begos 碰撞检测
                            // 与 粗略扫掠体 相交的 mapents 

    static std::multiset<double> tVals; 
                            //- 确认发生碰撞的 begos，将被收集起来，按照 t 值排序
};


#endif 

