/*
 * ===================== esrc_funcTypes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_FUNC_TYPES_H_
#define _TPR_FUNC_TYPES_H_

//-------------------- CPP --------------------//
#include <functional> 

//-------------------- Engine --------------------//
#include "GameObj.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//

using FUNC_V_V = std::function<void()>;
using F_GOID_GOPTR = std::function<void(goid_t,GameObj*)>;


}//---------------------- namespace: esrc -------------------------//
#endif 

