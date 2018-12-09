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


    u64      go_id; //- 玩家当前绑定的 go 的 id
    PixVec2  pos;   //- 在地图中的 坐标。


};


//-- 玩家数据 [内存态] --
//-  独一无二的存在，在 游戏中只有一份 
class Player{
public:
    explicit Player() = default;






private:

    GameObj go { "nullgo" }; //- 玩家当前绑定的 go
                            //- 游戏初始化时 绑定的是一个 名为 "nullgo" 的 go类型
                            //- 这是一个 空go 



};





#endif

