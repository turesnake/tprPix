/*
 * ====================== Player.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    存储所有 玩家 信息。
 *    ----------
 * ----------------------------
 */
#ifndef TPR_PLAYER_H
#define TPR_PLAYER_H

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "GameObjType.h" 
#include "ID_Manager.h" 
#include "SpeedLevel.h"
#include "InputINS.h"

// need:
class GameObj;


//-- 玩家数据 [内存态] --
//-  独一无二的存在，在 游戏中只有一份 
class Player{
public:
    Player() = default;

    //-- 必须等 chunks 彻底加载到 mem态（相伴的go数据也实例化）之后 --
    //   才能调用本函数 
    void bind_go( goid_t goid_ );
    void handle_inputINS( const InputINS &inputINS_ );

    GameObj &get_goRef() const;

    void set_moveSpeedLvl( SpeedLevel lvl_ )noexcept;

    //======== vals ========//
    goid_t    goid   {NULLID}; 
                    // 不再长期持有 goPtr，而是持有 goid，随用随取。
                    // 忽略这点性能损失
    goid_t   playerGoCircle_goid {NULLID};
                    // 这个 go实例 伴随 player实例 整个生命周期

private:
};


#endif

