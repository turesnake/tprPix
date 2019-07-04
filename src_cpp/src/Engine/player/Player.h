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

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "GameObj.h" 
#include "NineBox.h" 
#include "InputINS.h"



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


    //======== vals ========//
    goid_t    goid   {NULLID}; 
    //GameObj  *goPtr  {nullptr}; //- 玩家当前绑定的 go 指针 
    //std::shared_ptr<GameObj> goSPtr {}; //- 玩家当前绑定的 go 指针 
                    // 不再长期持有 goPtr，而是持有 goid，随用随取。
                    // 忽略这点性能损失

private:
};


#endif

