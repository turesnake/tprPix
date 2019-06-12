/*
 * ===================== esrc_gameSeed.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_gameSeed.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace gameSeed_inn {//-------- namespace: gameSeed_inn --------------//

    GameSeed  gameSeed {}; //- tmp, 游戏种子，
                            //- 暂设为：只有一个种子，且每次运行都重置

}//------------- namespace: gameSeed_inn end --------------//

GameSeed &get_gameSeed(){
    return gameSeed_inn::gameSeed;
}



}//---------------------- namespace: esrc -------------------------//


