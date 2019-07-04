/*
 * ========================= esrc_gameSeed.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_GAME_SEED_H
#define TPR_ESRC_GAME_SEED_H

//-------------------- Engine --------------------//
#include "GameSeed.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

void init_gameSeed();
GameSeed &get_gameSeed();


}//---------------------- namespace: esrc -------------------------//
#endif

