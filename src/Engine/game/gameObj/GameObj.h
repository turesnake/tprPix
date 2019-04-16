/*
 * ========================= GameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    GameObj 是游戏中的 一等公民。
 *    可以作为一个 独立的单位，存在于 游戏中
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_H_
#define _TPR_GAME_OBJ_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <unordered_map>

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
#include "Collision.h"
#include "GODirection.h"
#include "ActionSwitch.h" //- 将被取代...
#include "PubBinary.h"

#include "ActionFSM.h"

#include "chunkKey.h"




//--- 最基础的 go 类，就像一个 "伪接口" ----//
// 具象go类 并不用继承 基础go类，而是 “装配” 一个go实例，
// 通过  动态绑定 来配置 这个被绑定的 go实例的数据。
// 通过 function / bind 动态绑定各种回调函数
// 在 主引擎中，go类 是官方认可的通用类型。也只能通过这个 类来 访问 一切 go实例
//-------
//  并不存在孤立的 go实例，每个go实例，都被一个 具象go实例 所"表达“
//  goid 是全局唯一的。 其外层的 具象go实例 也使用这个 id号
//-------
//  go类 与 具象go类：
//  -- go类实例 负责存储实际的数据
//  -- 具象go类 只是一个 “装配工厂”，不存在 较长生命周期的 “具象go类实例”
class GameObj{
    using F_GO         = std::function<void(GameObj*)>;
    using F_PUB_BINARY = std::function<void(PubBinaryValType)>;
    using F_AFFECT     = std::function<void(GameObj*,GameObj*)>;
public:
    GameObj() = default;

    void init();//-- MUST --

    //-- disl <-> mem --//
    void        d2m( diskGameObj *_dgo );
    diskGameObj m2d();

    void reset_chunkKeys();
   

    inline void resize_pvtBinary( size_t _size ){
        this->pvtBinary.resize( _size );
    }
    inline u8_t *get_pvtBinaryPtr(){
        return &(this->pvtBinary.at(0));
    }

    GameObjMesh &creat_new_goMesh(  const std::string &_name,
                                    const std::string &_animFrameSetName,
                                    RenderLayerType    _layerType,
                                    ShaderProgram     *_pixShaderPtr,
                                    ShaderProgram     *_shadowShaderPtr,
                                    const glm::vec2   _pposOff,
                                    float             _off_z,
                                    bool              _isVisible,
                                    bool              _isCollide,
                                    bool              _isFlipOver );

    //-- 代表整个go实例 的 rootAnchorPos --
    //  放得非常深，通过多层调用才实现...
    //  rootAnchorPos 是静态数据，并不是 go当前 世界pos
    inline const AnchorPos &get_rootAnchorPos() const {
        return this->goMeshs.at("root").get_rootAnchorPos();
    }

    //-- 根据 direction，自动改写 isFlipOver --
    inline void set_isFlipOver_auto(){
        this->isFlipOver = (this->direction==GODirection::Left);    
    }

    //-- isPass 系列flag 也许不放在 collision 模块中...
    inline void set_collision_isDoPass( bool _b ){
        this->collision.isDoPass = _b;
    }
    inline void set_collision_isBePass( bool _b ){
        this->collision.isBePass = _b;
    }
    inline bool get_collision_isDoPass() const {
        return this->collision.isDoPass;
    }
    inline bool get_collision_isBePass() const {
        return this->collision.isBePass;
    }

    //- 获得 目标 ces 当前 绝对 altiRange
    //- 参数 _ces_altiRange 一般是在 碰撞检测流程中，从 mapent.major_gos 中取出的
    inline AltiRange get_currentAltiRange( const AltiRange &_ces_altiRange ){
        return ( _ces_altiRange + this->goPos.get_alti() );
    }

    inline const std::set<chunkKey_t> &get_chunkKeysRef(){
        return this->chunkKeys;
    }


    void debug(); //- 打印 本go实例 的所有信息

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
    F_AFFECT DoAffect_body {nullptr};
    F_AFFECT DoAffect_virtual {nullptr};
    F_AFFECT BeAffect_body {nullptr};   
        // 只有 body 会被登记到 mapent中，所以不存在 BeAffect_virtual

    //----------------- self vals ---------------//
    goid_t         id       {NULLID};    
    goSpecId_t     species  {0};                //- go species id
    GameObjFamily  family   {GameObjFamily::Major};  

    goid_t parentId {NULLID}; //- 不管是否为顶层go，都可以有自己的 父go。
                             //- 如果没有，此项写 NULLID
    
    float        weight    {0}; //- go重量 （影响自己是否会被 一个 force 推动）
    GODirection  direction {GODirection::Left};  //- 朝向

    //---- go 状态 ----//
    GameObjState      state     {GameObjState::Sleep};         //- 常规状态
    GameObjMoveState  moveState {GameObjMoveState::BeMovable}; //- 运动状态
    
    //--- move sys ---//
    GameObjPos   goPos {}; 
    Move         move  {};

    // - rootGoMesh  -- name = “root”; 核心goMesh;
    // - childGoMesh -- 剩下的goMesh
    std::unordered_map<std::string, GameObjMesh>  goMeshs {}; //- go实例 与 GoMesh实例 强关联
                            // 大部分go不会卸载／增加自己的 GoMesh实例
                            //- 在一个 具象go类实例 的创建过程中，会把特定的 GoMesh实例 存入此容器
                            //- 只存储在 mem态。 在go实例存入 硬盘时，GoMesh实例会被丢弃
                            //- 等再次从section 加载时，再根据 具象go类型，生成新的 GoMesh实例。

            // *** 此容器 疑似引发了一个 史诗级BUG... ***
            // 当在 具象类init() 中，调用 goMeshs.size() 等之类的语句时，程序就正常。
            // 但如果不调用，程序无法显示图形
            // 目前还没搞清原因
            // --- 这个 bug 暂时消失了... ---
            // 猜测的解法：cmake .. 将程序 彻底重编译一遍

    ActionSwitch    actionSwitch {}; //-- 将被取代...

    ActionFSM       actionFSM {};

    PubBinary       pubBinary {}; //- 动态变量存储区，此处的变量 可被 engine层/script层 使用

    //InputINS        inputINS  {}; //- gameKeys 指令组

    chunkKey_t      currentChunkKey {}; //- 本go 当前所在 chunk key
                                        //  在 本go被创建，以及每次move时，被更新
    

    //======== flags ========//
    bool    isTopGo   {true}; //- 是否为 顶层 go (有些go只是 其他go 的一部分)
    bool    isActive  {false}; //- 是否进入激活圈. 未进入激活圈的go，不参与逻辑运算，不被渲染
    bool    isDirty   {false};  //- 是否为 默认go（是否被改写过）
                            //- “默认go” 意味着这个 go没有被游戏改写过。
                            //- 当它跟着 mapSection 存入硬盘时，会被转换为 go_species 信息。
                            //- 以便少存储 一份 go实例，节省 硬盘空间。
    bool    isControlByPlayer  {false}; 
    bool    isFlipOver {false}; //- 图形左右翻转： false==不翻==向右； true==翻==向左；
                                //- 注意，这个值不应该由 具象go类手动配置
                                //  而应由 move／动画播放器 自动改写
                                // -- gmesh.isFlipOver 决定了 此图元的 静态方向
                                // -- go.isFlipOver    决定了 此图元 的动态方向，比如走动时
    
    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 go_id ( 在.cpp文件中初始化 )

private:

    //====== vals =====//
    std::set<chunkKey_t>  chunkKeys {}; //- 本go所有 collient 所在的 chunk 合集
                                        // 通过 reset_chunkKeys() 来更新。
                                        // 在 本go 生成时，以及每一次move时，都要更新这个 容器数据
                                        
                                        
    //----------- binary -------------//         
    std::vector<u8_t>  pvtBinary;  //- 只存储 具象go类 内部使用的 各种变量

    Collision    collision {}; //- 一个go实例，对应一个 collision实例。强关联

};



#endif

