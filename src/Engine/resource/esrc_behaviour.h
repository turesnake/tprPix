/*
 * ========================= esrc_behaviour.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_BEHAVIOUR_H_
#define _TPR_ESRC_BEHAVIOUR_H_

//-------------------- Engine --------------------//
#include "Behaviour.h" 

namespace esrc {//------------------ namespace: esrc -------------------------//

inline Behaviour behaviour {};  //- 全游戏唯一 Behaviour 实例
void call_scriptMain(); //- 调用 脚本层 入口函数

}//---------------------- namespace: esrc -------------------------//
#endif

