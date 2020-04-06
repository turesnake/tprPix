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


class GameObj;
//class UIObj;
class DyParam; 


//-- map自动生成器 使用的 uiInit函数 ---
using F_GO_INIT = std::function<void(GameObj&, const DyParam &dyParams_)>;
using F_GO      = std::function<void( GameObj& )>;
using F_AFFECT  = std::function<void( GameObj&, GameObj& )>; // params: dogoRef_, begoRef_


using goid_t        = uint64_t; //- gameObj id type
using goSpeciesId_t = uint64_t; //- gameObj species id type. = goSpeciesName.hash()
                             //  并不直接使用 size_t, 确保 数据库中数据 一定对其

//-- go move state／运动状态 --
enum class GameObjMoveState{
    AbsFixed  = 1, //- 固定于地面，绝对静止。 需要对齐于 像素
    BeMovable = 2, //- 静止，但可被移动（通过外部 forse 施加的影响）
                    // 从未被使用，将被废弃 ...

    Movable   = 3  //- 可移动。本go 可启动移动操作。
};

GameObjMoveState str_2_gameObjMoveState( const std::string &name_ )noexcept;




//-- go state / go 常规状态 --
enum class GameObjState{
    Sleep = 1, //- 休眠状态，不主动行动，（但可被动回应）
    Waked = 2  //- 活跃状态，主动发起行动。 
};

GameObjState str_2_gameObjState( const std::string &name_ )noexcept;




//- 三大 go 类群 --
//  这套系统使用使用，暂未确定
enum class GameObjFamily{
    Major   = 1, // 主go： 活体，树，建筑...
                 // 只有 MajorGo / BioSoup，可以参与 游戏世界的 碰撞检测
                 // 会被登记到对应 chunk 中 （从而可以跟随chunk 被释放）
                 // 会被登记到对应 mapent 中，从而可以进行 移动/技能 碰撞检测

    BioSoup,    // 异世界生物汤。
                // 只有 MajorGo / BioSoup，可以参与 游戏世界的 碰撞检测
                // 会被登记到对应 chunk 中 （从而可以跟随chunk 被释放）
                // 会被登记到对应 mapent 中，从而可以进行 移动/技能 碰撞检测
                // 特性上非常类似 普通的 mp-go
                

    Floor,      // 地面材质go （无法移动）
                // 会被登记到对应 chunk 中 （从而可以跟随chunk 被释放）
                // 不被登记到对应 mapent 中


    GroundGo,   // 折中产物，为地面铺设一层 eco 主体色 （无法移动）
                // 会被登记到对应 chunk 中 （从而可以跟随chunk 被释放）
                // 不被登记到对应 mapent 中


    WorldUI,    // 被放置在 游戏世界内 的 uiGo，
                // 比如跟随 活体移动的 血条/ 特效 等
                // 暂时只有 playerGoCircle.
                // 会被登记到对应 chunk 中 （从而可以跟随chunk 被释放）
                // 不被登记到对应 mapent 中
                // ---
                // 布恩 WorldUI-go 是要移动的，不要忘记更新 它们的 chunk登记信息!!! 


    UI,   // 被合并进 GO 的 UIGO 类， 无需执行 worldCoord 转换
          // 它们不应该出现在 游戏世界空间中，而独立存在于 ui空间 中 
          // 如果某个 ui-go，需要被放置在 游戏世界空间 内，应被设置为 WorldUI
          // ---
          // 由于 UI-go 并不存在于 游戏世界空间，所以和 chunk/mapent 完全无关

};

GameObjFamily str_2_gameObjFamily( const std::string &name_ )noexcept;





#endif

