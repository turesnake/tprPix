/*
 * ========================= esrc_behaviour.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_BEHAVIOUR_H
#define TPR_ESRC_BEHAVIOUR_H

//-------------------- Engine --------------------//
#include "Behaviour.h" 

namespace esrc {//------------------ namespace: esrc -------------------------//

void init_behaviour();
Behaviour &get_behaviour();
void call_scriptMain(); //- 调用 脚本层 入口函数

}//---------------------- namespace: esrc -------------------------//
#endif

