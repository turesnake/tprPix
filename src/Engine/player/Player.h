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
#ifndef _TPR_PLAYER_H_
#define _TPR_PLAYER_H_

//------------------- C --------------------//
#include <cassert>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "GameObj.h" 
#include "NineBox.h" 
#include "InputINS.h"


//-- 玩家数据 [硬盘态] --
//-  独一无二的存在，在硬盘中只有一份 
struct diskPlayer{
    u64_t      go_id {}; //- 玩家当前绑定的 go 的 id
    i32_t      posx {}; //- 在地图中的 坐标。
    i32_t      posy {}; //- 在地图中的 坐标。
};


//-- 玩家数据 [内存态] --
//-  独一无二的存在，在 游戏中只有一份 
class Player{
public:
    Player() = default;

    //-- 延迟初始化 --
    void init();

    //-- 硬盘态 - 内存态 转换 --
    void d2m( diskPlayer *_dp );
    diskPlayer m2d();

    //-- 必须等 section 彻底加载到 mem态（相伴的go数据也实例化）之后 --
    //   才能调用本函数 
    void bind_goPtr();

    inline void bind_goid( goid_t _goid ){
        assert( _goid != NULLID );
        this->goid = _goid;
    }


    void handle_inputINS( const InputINS &_inputINS );

    //======== vals ========//
    goid_t    goid   {NULLID}; 
    GameObj  *goPtr {nullptr}; //- 玩家当前绑定的 go 指针            
private:
};


#endif

