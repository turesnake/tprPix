/*
 * ====================== Player.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    存储所有 玩家 信息。
 *    ----------
 *    
 * ----------------------------
 */
#ifndef _TPR_PLAYER_H_
#define _TPR_PLAYER_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- SELF --------------------//
#include "PixVec.h" 
#include "GameObj.h"


//-- 玩家数据 [硬盘态] --
//-  独一无二的存在，在硬盘中只有一份 
struct diskPlayer{

    u64      go_id {}; //- 玩家当前绑定的 go 的 id
    i32      posx {}; //- 在地图中的 坐标。
    i32      posy {}; //- 在地图中的 坐标。

};


//-- 玩家数据 [内存态] --
//-  独一无二的存在，在 游戏中只有一份 
class Player{
public:
    explicit Player() = default;

    //-- 延迟初始化 --
    void init();

    //-- 硬盘态 - 内存态 转换 --
    void d2m( diskPlayer *_dp );
    diskPlayer m2d();

    //------- vals --------//




private:

    GameObj go { "nullgo" }; //- 玩家当前绑定的 go
                            //- 游戏初始化时 绑定的是一个 名为 "nullgo" 的 go类型
                            //- 这是一个 空go 



};





#endif

