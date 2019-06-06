/*
 * ========================= esrc_player.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_PLAYER_H_
#define TPR_ESRC_PLAYER_H_

//-------------------- Engine --------------------//
#include "Player.h" 

namespace esrc {//------------------ namespace: esrc -------------------------//

//--- mem ---//
//inline Player  player {}; //- 全游戏唯一 Player 实例  

Player &get_player();

}//---------------------- namespace: esrc -------------------------//
#endif

