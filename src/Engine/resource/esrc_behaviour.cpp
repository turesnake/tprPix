/*
 * ========================= esrc_behaviour.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_behaviour.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace behaviour_inn {//------------------ namespace: behaviour_inn -------------------------//

    Behaviour behaviour {};  //- 全游戏唯一 Behaviour 实例

}//--------------------- namespace: behaviour_inn end -------------------------//


Behaviour &get_behaviour(){
    return behaviour_inn::behaviour;
}


}//---------------------- namespace: esrc -------------------------//
