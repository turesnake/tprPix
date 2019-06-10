/*
 * ======================= esrc_player.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_player.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace {//-------- namespace: --------------//

    Player  player {}; //- 全游戏唯一 Player 实例  

}//------------- namespace: end --------------//


Player &get_player(){
    return player;
}

}//---------------------- namespace: esrc -------------------------//

