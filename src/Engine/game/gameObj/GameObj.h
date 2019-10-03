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

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <unordered_map>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "GameObjType.h" 
#include "GameObjMesh.h" 
#include "ID_Manager.h" 
#include "IntVec.h" 
#include "Move.h"
#include "MapCoord.h" 
#include "GameObjPos.h"
#include "UIAnchor.h"
#include "Collision.h"
#include "ColliDataFromJ.h"
#include "ActionSwitch.h" //- 将被取代...
#include "PubBinary2.h"
#include "ActionFSM.h"
#include "chunkKey.h"
#include "animSubspeciesId.h"


//--- 一个仍在建设中的 大杂烩 ----//
// 具象go类 并不用继承 基础go类，而是 “装配” 一个go实例，
// 通过 function / bind 动态绑定各种回调函数
// 在 主引擎中，go类 是官方认可的通用类型。也只能通过这个 类来 访问 一切 go实例
//-------
//  并不存在孤立的 go实例，每个go实例，都被一个 具象go实例 所"表达“
//  goid 是全局唯一的。 其外层的 具象go实例 也使用这个 id号
class GameObj : public std::enable_shared_from_this<GameObj>{
    using F_GO         = std::function<void( GameObj& )>;
    using F_AFFECT     = std::function<void( GameObj&, GameObj& )>;
    using F_void         = std::function<void()>;
    using F_void_double  = std::function<void( double )>;
    using F_double       = std::function<double()>;
    using F_c_dvec2Ref   = std::function<const glm::dvec2 &()>;
    using F_void_c_dvec2Ref = std::function<void(const glm::dvec2 &)>;

public:
    //-- factory --
    static std::shared_ptr<GameObj> factory_for_regularGo(    goid_t goid_, 
                                                            const glm::dvec2 &dpos_ )noexcept{
        std::shared_ptr<GameObj> goSPtr( new GameObj(goid_) );//- can not use make_shared
        goSPtr->init_for_regularGo( dpos_ );
        return goSPtr;
    }

    static std::shared_ptr<GameObj> factory_for_uiGo(    goid_t goid_, 
                                                        const glm::dvec2 &basePointProportion_,
                                                        const glm::dvec2 &offDPos_ )noexcept{
        std::shared_ptr<GameObj> goSPtr( new GameObj(goid_) );//- can not use make_shared
        goSPtr->init_for_uiGo( basePointProportion_, offDPos_ );
        return goSPtr;
    }

    size_t reCollect_chunkKeys();

    void rebind_rootAnimActionPosPtr();
   

    //----- pvtBinary -----//
    template< typename T >
    inline T *init_pvtBinary()noexcept{
        this->pvtBinary.resize( sizeof(T), 0 );
        return reinterpret_cast<T*>( &(this->pvtBinary.at(0)) );
    }
    template< typename T >
    inline T *get_pvtBinaryPtr()noexcept{
        tprAssert( sizeof(T) == this->pvtBinary.size() );
        return reinterpret_cast<T*>( &(this->pvtBinary.at(0)) );
    }

    GameObjMesh &creat_new_goMesh(  const std::string &name_,
                            animSubspeciesId_t  subspeciesId_,
                            const std::string   &actionName_,
                            RenderLayerType     layerType_,
                            ShaderProgram       *pixShaderPtr_,
                            const glm::vec2     pposOff_ = glm::vec2{0.0,0.0},
                            double              off_z_ = 0.0,
                            bool                isVisible_ = true );

    void init_check(); //- call in end of go init 


    //--------- collison ----------//
    //-- isPass 系列flag 也许不放在 collision 模块中...
    //   如果一个 没有加载 collide 组件的 go实例，调用这系列函数，将直接报错...(不是好办法)
    inline void set_collision_isDoPass( bool b_ )noexcept{ this->collisionUPtr->set_isDoPass(b_); }
    inline void set_collision_isBePass( bool b_ )noexcept{ this->collisionUPtr->set_isBePass(b_); }
    inline bool get_collision_isDoPass() const noexcept{ return this->collisionUPtr->get_isDoPass(); }
    inline bool get_collision_isBePass() const noexcept{ return this->collisionUPtr->get_isBePass(); }
    inline glm::dvec2 detect_collision_for_move( const glm::dvec2 &speedVal_ )noexcept{ return this->collisionUPtr->detect_for_move( speedVal_ ); }
    inline const std::set<IntVec2> &get_currentSignINMapEntsRef() const noexcept{ return this->collisionUPtr->get_currentSignINMapEntsRef(); }

    inline ColliderType get_colliderType()const noexcept{ return this->colliDataFromJPtr->get_colliderType(); }


    //--------- rootAnimActionPos ----------//
    inline const AnimActionPos &get_rootAnimActionPosRef()const noexcept{
            tprAssert( this->rootAnimActionPosPtr );
        return *(this->rootAnimActionPosPtr);
    }
    inline Circular calc_circular( CollideFamily family_ )const noexcept{
        return this->colliDataFromJPtr->calc_circular( this->get_dpos(), family_ );
    }
    inline Capsule calc_capsule( CollideFamily family_ )const noexcept{
        return this->colliDataFromJPtr->calc_capsule( this->get_dpos(), family_ );
    }

    inline GoAltiRange get_currentGoAltiRange()noexcept{
        return (this->get_rootAnimActionPosRef().get_lGoAltiRange() + this->get_pos_alti());
    }

    inline const std::set<chunkKey_t> &get_chunkKeysRef()noexcept{
            tprAssert( this->isMoveCollide ); //- tmp
        return this->chunkKeys;
    }
    inline bool find_in_chunkKeys( chunkKey_t chunkKey_ ) const noexcept{
        return (this->chunkKeys.find(chunkKey_) != this->chunkKeys.end());
    }

    inline GameObjMesh &get_goMeshRef( const std::string &name_ )noexcept{
            tprAssert( this->goMeshs.find(name_) != this->goMeshs.end() ); //- tmp
        return *(this->goMeshs.at(name_).get());
    }

    inline void set_actionDirection( NineDirection dir_ )noexcept{ this->actionDirection = dir_; }
    inline NineDirection get_actionDirection()const noexcept{ return this->actionDirection; }


    void debug();

    //-- pos sys --    
    F_void_c_dvec2Ref accum_dpos  {nullptr};
    F_void_double     set_pos_alti         {nullptr};
    F_c_dvec2Ref      get_dpos      {nullptr};
    F_double          get_pos_alti         {nullptr};
    

    inline void render_all_goMesh()noexcept{
        for( auto &pairRef : this->goMeshs ){
            pairRef.second->RenderUpdate_auto();
        }
    }
    //-- now, just used in GroundGo --
    inline void render_all_groundGoMesh()noexcept{
        for( auto &pairRef : this->goMeshs ){
            pairRef.second->RenderUpdate_ground();
        }
    }
    

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

    //----------------- self vals ---------------//
    goid_t         id;  
    goSpecId_t     species  {0};                //- go species id
    GameObjFamily  family   {GameObjFamily::Major};  

    goid_t parentId {NULLID}; //- 不管是否为顶层go，都可以有自己的 父go。
                              //- 暂未被使用
    
    double        weight    {0}; //- go重量 （影响自己是否会被 一个 force 推动）

    



    //---- go 状态 ----//
    GameObjState      state     {GameObjState::Sleep};         //- 常规状态
    GameObjMoveState  moveState {GameObjMoveState::BeMovable}; //- 运动状态
    
    //--- move sys ---//
    Move         move;


    ActionSwitch    actionSwitch; //-- 将被 ActionFSM 取代...
    ActionFSM       actionFSM {}; //- 尚未完工...

    //PubBinary       pubBinary {}; //- 动态变量存储区，此处的变量 可被 engine层/script层 使用
    PubBinary2      pubBinary {};  //- 简易版，存储所有元素

    //InputINS        inputINS  {}; //- gameKeys 指令组

    chunkKey_t      currentChunkKey {}; //- 本go 当前所在 chunk key
                                        //  在 本go被创建，以及每次move时，被更新
    

    //======== flags ========//
    bool    isTopGo   {true}; //- 是否为 顶层 go (有些go只是 其他go 的一部分)
                                //  目前这个值 暂未被使用 ...

    bool    isActive  {false}; //- 是否进入激活圈. 未进入激活圈的go，不参与逻辑运算，不被渲染
    bool    isDirty   {false};  //- 是否为 默认go（是否被改写过）
                            //- “默认go” 意味着这个 go没有被游戏改写过。
                            //- 当它跟着 mapSection 存入硬盘时，会被转换为 go_species 信息。
                            //- 以便少存储 一份 go实例，节省 硬盘空间。
    bool    isControlByPlayer  {false}; 

    //bool    isFlipOver {false}; //- 图形左右翻转： false==不翻==向右； true==翻==向左；
                                //- 注意，这个值不应该由 具象go类手动配置
                                //  而应由 move／动画播放器 自动改写
                                // -- gmesh.isFlipOver 决定了 此图元的 静态方向
                                // -- go.isFlipOver    决定了 此图元 的动态方向，比如走动时
                                // ----

                                // 在新视觉风格中，可能被取代....

    bool    isMoveCollide {false};  //- 是否参与 移动碰撞检测，
                                    //  取代 rootGoMesh.isCollide 
                                    //  若为 false，也不需要 登记到 mapent 上
                                    //  值为 false 的go，会被 精简掉 大量代码
                                    //  uiGo 一律为 false。部分 regularGo 也可为 false（游戏世界中的ui元素）

    
    //======== static ========//
    static ID_Manager  id_manager;

private:

    GameObj( goid_t goid_ ):
        id(goid_),
        move( *this ),
        actionSwitch( *this )
        {}

    //-- init --//
    void init_for_regularGo( const glm::dvec2 &dpos_ );
    void init_for_uiGo( const glm::dvec2 &basePointProportion_,
                        const glm::dvec2 &offDPos_ );

    //====== vals =====//
    std::set<chunkKey_t>  chunkKeys {}; //- 本go所有 collient 所在的 chunk 合集
                                        // 通过 reCollect_chunkKeys() 来更新。
                                        // 在 本go 生成时，以及 rootCES 每一次步进时，都要更新这个 容器数据

                                        // isMoveCollide == false 的go，此容器永远为空


    // - rootGoMesh  -- name = “root”; 核心goMesh;
    // - childGoMesh -- 剩下的goMesh
    std::unordered_map<std::string, std::unique_ptr<GameObjMesh>>  goMeshs {};
                            //- go实例 与 GoMesh实例 强关联
                            // 大部分go不会卸载／增加自己的 GoMesh实例
                            //- 在一个 具象go类实例 的创建过程中，会把特定的 GoMesh实例 存入此容器
                            //- 只存储在 mem态。 在go实例存入 硬盘时，GoMesh实例会被丢弃
                            //- 等再次从section 加载时，再根据 具象go类型，生成新的 GoMesh实例。

    //====== pos sys =====//
    // only one will be used
    std::unique_ptr<GameObjPos> goPosUPtr   {nullptr};
    std::unique_ptr<UIAnchor>   uiGoPosUPtr {nullptr};


    //GODirection  direction {GODirection::Left};  //- 朝向
    NineDirection   actionDirection {NineDirection::Mid};  //- 角色 动画朝向


    //----------- pvtBinary -------------//         
    std::vector<u8_t>  pvtBinary {};  //- 只存储 具象go类 内部使用的 各种变量

    std::unique_ptr<Collision> collisionUPtr {nullptr};
    const ColliDataFromJ      *colliDataFromJPtr {nullptr}; // 一经init，永不改变


    const AnimActionPos *rootAnimActionPosPtr {nullptr};
};



#endif

