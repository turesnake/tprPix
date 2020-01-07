/*
 * ===================== GameObjType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    GameObj 各种数据 结构
 * ----------------------------
 */
#ifndef TPR_GAME_OBJ_TYPE_H
#define TPR_GAME_OBJ_TYPE_H

//-------------------- CPP --------------------//
#include <string>
#include <functional>

//------------------- Libs --------------------//
#include "tprDataType.h" 



class GameObj;
class UIObj;
class DyParam; 


//-- map自动生成器 使用的 uiInit函数 ---
using F_GO_INIT = std::function<void(GameObj&, const DyParam &dyParams_)>;
using F_GO      = std::function<void( GameObj& )>;
using F_AFFECT  = std::function<void( GameObj&, GameObj& )>; // params: dogoRef_, begoRef_


using goid_t        = u64_t; //- gameObj id type
using goSpeciesId_t = u64_t; //- gameObj species id type. = goSpeciesName.hash()
                             //  并不直接使用 size_t, 确保 数据库中数据 一定对其

//-- go move state／运动状态 --
enum class GameObjMoveState{
    AbsFixed  = 1, //- 固定于地面，绝对静止。 需要对齐于 像素
    BeMovable = 2, //- 静止，但可被移动（通过外部 forse 施加的影响）
                    // 从未被使用，将被废弃 ...

    Movable   = 3  //- 可移动。本go 可启动移动操作。
};

GameObjMoveState str_2_GameObjMoveState( const std::string &name_ )noexcept;




//-- go state / go 常规状态 --
enum class GameObjState{
    Sleep = 1, //- 休眠状态，不主动行动，（但可被动回应）
    Waked = 2  //- 活跃状态，主动发起行动。 
};

GameObjState str_2_GameObjState( const std::string &name_ )noexcept;




//- 三大 go 类群 --
//  这套系统使用使用，暂未确定
enum class GameObjFamily{
    Major   = 1, //- 主go： 活体，树，建筑...
                 //  只有 major go 可以参与 游戏世界的 碰撞检测

    Floor,     
    GroundGo,   //  折中产物，为地面铺设一层 eco 主体色

    Oth,   // 暂时只有 playerGoCircle

    UI,   // 被合并进 GO 的 UIGO 类， 无需执行 worldCoord 转换
          // 它们不应该出现在 游戏世界空间中，而独立存在于 ui空间 中 
          // 对于另一种，身处游戏世界空间，但扮演 ui功能的 go，
          // 暂未为其 分配 类型...
};

GameObjFamily str_2_GameObjFamily( const std::string &name_ )noexcept;





#endif

