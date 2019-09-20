/*
 * ======================= Collision.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  第三版
 * ---------
 */
#ifndef TPR_COLLISION_2_H
#define TPR_COLLISION_2_H

//-------------------- CPP --------------------//
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <utility> //pair

//-------------------- Engine --------------------//
#include "NineBox.h"
#include "ColliderType.h"
#include "GameObjPos.h"
#include "GameObjType.h"
#include "SignInMapEnts.h"


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
public:
    Collision( GameObj &goRef_ ):
        goRef(goRef_)
        {}

    glm::dvec2 detect_for_move( const glm::dvec2 &moveVec_ );


    //-- only call in go init --
    inline void init_signInMapEnts( const glm::dvec2 &newRootAnchorDPos_,
                                    F_get_colliPointDPosOffsRef func_1_ )noexcept{
        this->signInMapEntsUPtr = std::make_unique<SignInMapEnts>();
        this->signInMapEntsUPtr->init_datas( newRootAnchorDPos_, func_1_ );
    }

    inline void set_isDoPass( bool val_ )noexcept{ this->isDoPass = val_; }
    inline void set_isBePass( bool val_ )noexcept{ this->isBePass = val_; }
    inline bool get_isDoPass() const noexcept{ return this->isDoPass; }
    inline bool get_isBePass() const noexcept{ return this->isBePass; }
    inline const std::set<IntVec2> &get_currentSignINMapEntsRef() const noexcept{
            tprAssert( this->signInMapEntsUPtr );
        return this->signInMapEntsUPtr->get_currentSignINMapEntsRef();
    }


private:

    void collect_adjacentBeGos();
    glm::dvec2 detect_adjacentBeGos( const glm::dvec2 &moveVec_ );

    void handle_chunkKeys();

    
    std::pair<bool, glm::dvec2> for_move_inn( const glm::dvec2 &moveVec_ );


    //======== vals ========//
    GameObj    &goRef;

    std::unique_ptr<SignInMapEnts> signInMapEntsUPtr {nullptr};

    //-- 以下容器，仅在函数内部使用 --
    std::unordered_map<goid_t, glm::dvec2> adjacentBeGos {}; // 相邻begos
                            // --1-- begoid
                            // --2-- self_2_oth
                            // 相邻go 是触发 ”滑动式位移“ 的唯一途径

    std::unordered_set<goid_t> begoids {}; //- 从 signInMapEnts 中收集的 半有效 begoids
    std::multimap<double,goid_t> tbegoids {}; //- 确认发生碰撞的 begos，将被收集起来，按照 t 值排序


    //======== flags ========//
    //  下方这组 可能存在 逻辑漏洞，但先这么用着... 
    bool  isDoPass  {false};//- 当自己为 主动go 时，是否穿过 被动go；
                            //  如果本go 是 “穿透箭”。可将此值设置为 true
                            //  此时，本go 将无视 被动go 的状态，移动穿过一切 被动go
    bool  isBePass  {false};//- 当自己为 被动go 时，是否允许 主动go 穿过自己；
                            //  如果本go 是 “草”／“腐蚀液”，可将此值 设置为 true
                            //  此时，本go 将无法阻止任何 go 从自己身上 穿过
};


#endif 

