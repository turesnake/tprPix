/*
 * ========================= GameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   Fst-class Citizens in game
 * ----------------------------
 */
#ifndef TPR_GAME_OBJ_H
#define TPR_GAME_OBJ_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <variant>

//-------------------- Engine --------------------//
#include "functorTypes.h"
#include "GameObjType.h" 
#include "GameObjMeshSet.h" 
#include "goLabelId.h"
#include "ID_Manager.h" 
#include "Move.h"
#include "MapCoord.h" 
#include "GameObjPos.h"
#include "UIAnchor.h"
#include "Collision.h"
#include "ColliDataFromJson.h"
#include "ActionSwitch.h" //- 将被取代...
#include "PubBinary2.h"
#include "ActionFSM.h"
#include "chunkKey.h"
#include "animSubspeciesId.h"
#include "DyBinary.h"
#include "BrokenLvl.h"
#include "History.h"
#include "GoFunctorSet.h"
#include "ShaderType.h"
#include "SignInMapEnts_Square.h"


//--- 一个仍在建设中的 丑陋的 大杂烩 ----//
// 具象go类 并不用继承 基础go类，而是 “装配” 一个go实例，
// 通过 function / bind 动态绑定各种回调函数
// 在 主引擎中，go类 是官方认可的通用类型。也只能通过这个 类来 访问 一切 go实例
//-------
//  并不存在孤立的 go实例，每个go实例，都被一个 具象go实例 所"表达“
//  goid 是全局唯一的。 其外层的 具象go实例 也使用这个 id号
class GameObj : public std::enable_shared_from_this<GameObj>{
public:
    //-- factory --
    static std::shared_ptr<GameObj> factory_for_regularGo(  goid_t goid_, 
                                                            const glm::dvec2 &dpos_,
                                                            size_t  goUWeight_ )noexcept{
        std::shared_ptr<GameObj> goSPtr( new GameObj(goid_, goUWeight_) );//- can not use make_shared
        goSPtr->init_for_regularGo( dpos_ );
        return goSPtr;
    }

    static std::shared_ptr<GameObj> factory_for_uiGo(   goid_t goid_, 
                                                        const glm::dvec2 &basePointProportion_,
                                                        const glm::dvec2 &offDPos_,
                                                        size_t  goUWeight_ )noexcept{
        std::shared_ptr<GameObj> goSPtr( new GameObj(goid_, goUWeight_) );//- can not use make_shared
        goSPtr->init_for_uiGo( basePointProportion_, offDPos_ );
        return goSPtr;
    }

    size_t reCollect_chunkKeys();

    void rebind_rootAnimActionPosPtr();
   
    //----- pvtBinary -----//
    template< typename T >
    inline T *init_pvtBinary()noexcept{ return this->pvtBinary.init<T>(); }
    template< typename T >
    inline T *get_pvtBinaryPtr()noexcept{ return this->pvtBinary.get<T>(); }
    template< typename T >
    inline const T *get_pvtBinaryPtr()const noexcept{ return this->pvtBinary.get<T>(); }
    

    void init_check(); //- call in end of go init 


    //--------- rootAnimActionPos ----------//
    inline Circular calc_circular( CollideFamily family_ )const noexcept{
        return this->colliDataFromJsonPtr->calc_circular( this->get_dpos(), family_ );
    }
    inline Square calc_square()const noexcept{
        return this->colliDataFromJsonPtr->calc_square( this->get_dpos() );
    }

    inline GoAltiRange get_currentGoAltiRange()noexcept{
        return (this->get_pos_lAltiRange() + this->get_pos_alti());
    }

    inline bool find_in_chunkKeys( chunkKey_t chunkKey_ ) const noexcept{
        return (this->chunkKeys.find(chunkKey_) != this->chunkKeys.end());
    }

    inline void insert_2_childGoIds( goid_t id_ )noexcept{
        auto [insertIt, insertBool] = this->childGoIds.insert(id_); 
        tprAssert( insertBool );
    }


    //-- 确保 所有 History 变量，若存在更新，都已被同步
    //   每一渲染帧 末尾调用
    inline void make_sure_all_historyVals_is_synced()const noexcept{
        tprAssert( !this->actionDirection.get_isDirty() );
        tprAssert( !this->brokenLvl.get_isDirty() );
        tprAssert( !this->move.moveSpeedLvl.get_isDirty() );
        tprAssert( !this->move.crawlDirAxes.get_isDirty() );
        //...
    }

    //---------------- set -----------------//
    inline void set_parentGoId( goid_t id_ )noexcept{ this->parentGoId = id_; }
    inline void set_colliDataFromJsonPtr( const ColliDataFromJson *ptr_ )noexcept{ this->colliDataFromJsonPtr = ptr_; }

    //---------------- get -----------------//
    inline bool                         get_isMoving()const noexcept{ return this->move.get_isMoving(); } // 若为 false，需对齐到 像素
    inline const std::set<chunkKey_t>   &get_chunkKeysRef()noexcept{ return this->chunkKeys; }
    inline ColliderType                 get_colliderType()const noexcept{ return this->colliDataFromJsonPtr->get_colliderType(); }
    inline goid_t                       get_parentGoId()const noexcept{ return this->parentGoId; }
    inline const std::set<goid_t>       &get_childGoIdsRef()const noexcept{ return this->childGoIds; }
    inline size_t                       get_goUWeight()const noexcept{ return this->goUWeight; }


    inline Collision &get_collisionRef()noexcept{ 
            tprAssert(this->collisionUPtr); // tmp
        return *(this->collisionUPtr); 
    }

    inline const SignInMapEnts_Square &get_signInMapEnts_square_ref()const noexcept{
        return SignInMapEnts_Square::get_signInMapEnts_square_ref( this->colliDataFromJsonPtr->get_signInMapEnts_square_type() );
    }


    void debug();


    //-- pos sys --    
    std::function<void(const glm::dvec2 &)>     accum_dpos          {nullptr};
    std::function<void(double)>                 set_pos_alti        {nullptr};
    std::function<void(GoAltiRange)>            set_pos_lAltiRange  {nullptr};
    std::function<const glm::dvec2 &()>         get_dpos            {nullptr};
    std::function<double()>                     get_pos_alti        {nullptr};
    std::function<GoAltiRange()>                get_pos_lAltiRange  {nullptr};

    //---------------- callback -----------------//
    // 这些 函数对象 可以被放入 private,然后用 函数调用来 实现绑定...
    F_GO  Awake {nullptr};  //- unused
    F_GO  Start {nullptr};  //- unused

    F_GO  RenderUpdate {nullptr}; //- 每1渲染帧，被引擎调用
    F_GO  LogicUpdate  {nullptr}; //- 每1逻辑帧，被主程序调用 （帧周期未定）

    F_GO  BeAffect     {nullptr}; //- 当 本go实例 被外部 施加技能／影响 时，调用的函数
                                  //- 未来可能会添加一个 参数：“被施加技能的类型”
                                  // 这个 函数对象，可能会被整合到全新的 Affect 系统中...

    //-- tmp,未来会被整理..
    // 当某一个未绑定时，应将其设置为 nullptr,这样 碰撞检测系统会跳过它
    F_AFFECT DoAffect_body    {nullptr};
    F_AFFECT DoAffect_virtual {nullptr};
    F_AFFECT BeAffect_body    {nullptr};   
        // 只有 body 会被登记到 mapent中，所以不存在 BeAffect_virtual

    //=============== Self Values ===============//
    goid_t              goid;  
    goSpeciesId_t       speciesId  {0};
    goLabelId_t         goLabelId  {};
    GameObjFamily       family   {GameObjFamily::Major};  
                            
    double              weight    {0}; //- go重量 （影响自己是否会被 一个 force 推动）

    //---- go 状态 ----//
    GameObjState        state     {GameObjState::Sleep};         //- 常规状态
    GameObjMoveState    moveState {GameObjMoveState::AbsFixed}; //- 运动状态


    //---- history vals ----//
    // stone val-state at last render-frame
    History<NineDirection> actionDirection { NineDirection::Center };
                                    //- 角色 动画朝向
                                    // 此值，仅指 go 在 window坐标系上的 朝向（视觉上看到的朝向）
                                    // 而不是在 worldCoord 中的朝向

    History<BrokenLvl>  brokenLvl       { BrokenLvl::Lvl_0 }; 
                                    // 破损等级，0为完好。
                                    // 当部分go（比如地景）遭到破坏时，此值也会跟着被修改，
                                    // 进一步会影响其 外貌
                                    // 未实现 ...

    //---
    Move         move;

    ActionSwitch    actionSwitch; //-- 将被 ActionFSM 取代...
    ActionFSM       actionFSM {}; //- 尚未完工...

    GameObjMeshSet  goMeshSet;

    //PubBinary       pubBinary {}; //- 动态变量存储区，此处的变量 可被 engine层/script层 使用
    PubBinary2      pubBinary {};  //- 简易版，存储所有元素, 仅用于测试 ...
                                // in future, use DyBinary

    //InputINS        inputINS  {}; //- gameKeys 指令组

    chunkKey_t      currentChunkKey {}; //- 本go 当前所在 chunk key
                                        //  在 本go被创建，以及每次move时，被更新

    GoFunctorSet    pubFunctors {}; // 存储一组，通过 GoFunctorLabel 索引的 任意类型的函数对象


     

    //======== flags ========//
    bool    isTopGo   {true}; //- 是否为 顶层 go (有些go只是 其他go 的一部分)
                                //  目前这个值 暂未被使用 ...

    bool    isActive  {false}; //- 是否进入激活圈. 未进入激活圈的go，不参与逻辑运算，不被渲染
    bool    isDirty   {false};  //- 是否为 默认go（是否被改写过）
                            //- “默认go” 意味着这个 go没有被游戏改写过。
                            //- 当它跟着 mapSection 存入硬盘时，会被转换为 go_species 信息。
                            //- 以便少存储 一份 go实例，节省 硬盘空间。
    bool    isControlByPlayer  {false}; 

    bool    isMoveCollide {false};  //- 是否参与 移动碰撞检测，
                                    //  uiGo 一律为 false。部分 majorGo 也可为 false（游戏世界中的ui元素）
                                    //  ---
                                    //  所有 majorGo 都要登记到 map 上，不管本值是否为 true

    
    //======== static ========//
    static ID_Manager  id_manager;

private:
    GameObj(    goid_t goid_,
                size_t goUWeight_ ):
        goid(goid_),
        move(*this),
        actionSwitch(*this),
        goMeshSet(*this),
        goUWeight(goUWeight_)
        {}

    //-- init --//
    void init_for_regularGo( const glm::dvec2 &dpos_ );
    void init_for_uiGo( const glm::dvec2 &basePointProportion_,
                        const glm::dvec2 &offDPos_ );

    //======== vals =======//

    goid_t              parentGoId {NULLID};  // 不管是否为顶层go，都可以有自己的 父go。
    std::set<goid_t>    childGoIds {};        // 可为空 
    size_t              goUWeight;


    std::set<chunkKey_t>  chunkKeys {}; //- 本go所有 collient 所在的 chunk 合集
                                        // only used for majorGos
                                        // 通过 reCollect_chunkKeys() 来更新。
                                        // 在 本go 生成时，以及 每一次移动时，都要更新这个 容器数据

    //====== pos sys =====//
    std::variant<   std::monostate, // 当变量为空时，v.index() 返回 0
                    std::unique_ptr<GameObjPos>, 
                    std::unique_ptr<UIAnchor>> goPosVUPtr {};


    //----------- pvtBinary -------------//             
    DyBinary    pvtBinary {};//- 只存储 具象go类 内部使用的 各种变量

    std::unique_ptr<Collision>  collisionUPtr {nullptr};
    const ColliDataFromJson     *colliDataFromJsonPtr {nullptr}; // 一经init，永不改变

};


#endif

