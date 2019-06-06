/*
 * ========================= behaviour_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_behaviour.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace {//------------------ namespace -------------------------//

    Behaviour behaviour {};  //- 全游戏唯一 Behaviour 实例

}//--------------------- namespace: end -------------------------//


Behaviour &get_behaviour(){
    return behaviour;
}


}//---------------------- namespace: esrc -------------------------//
