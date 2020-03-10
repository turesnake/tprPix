/*
 * ========================= esrc_behaviour.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_behaviour.h"
#include "esrc_state.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace behaviour_inn {//------------------ namespace: behaviour_inn -------------------------//
    std::unique_ptr<Behaviour> behaviourPtr; //- only-one Behaviour
}//--------------------- namespace: behaviour_inn end -------------------------//

void init_behaviour(){
    behaviour_inn::behaviourPtr = std::make_unique<Behaviour>();
    esrc::insertState("behaviour");
}


Behaviour &get_behaviour(){
    return *(behaviour_inn::behaviourPtr);
}


}//---------------------- namespace: esrc -------------------------//
