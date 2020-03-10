/*
 * ======================= esrc_player.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_player.h"
#include "esrc_state.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

namespace player_inn {//-------- namespace: player_inn --------------//
 
    std::unique_ptr<Player> playerUPtr;

}//------------- namespace: player_inn end --------------//


void init_player(){
    player_inn::playerUPtr = std::make_unique<Player>();
    esrc::insertState("player");
}


Player &get_player(){
    return *(player_inn::playerUPtr);
}

}//---------------------- namespace: esrc -------------------------//

