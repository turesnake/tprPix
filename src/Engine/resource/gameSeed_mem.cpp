/*
 * ===================== gameSeed_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_gameSeed.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace {//-------- namespace: --------------//

    GameSeed  gameSeed {}; //- tmp, 游戏种子，
                            //- 暂设为：只有一个种子，且每次运行都重置

}//------------- namespace: end --------------//

GameSeed &get_gameSeed(){
    return gameSeed;
}



}//---------------------- namespace: esrc -------------------------//


