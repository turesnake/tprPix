/*
 * ===================== esrc_funcTypes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_FUNC_TYPES_H
#define TPR_FUNC_TYPES_H

//-------------------- CPP --------------------//
#include <functional> 
#include <memory>

//-------------------- Engine --------------------//
#include "GameObjType.h"

class GameObj;
class UIObj;


namespace esrc {//------------------ namespace: esrc -------------------------//

using FUNC_V_V = std::function<void()>;
using F_GOID_GOPTR = std::function<void(goid_t, GameObj&)>;


}//---------------------- namespace: esrc -------------------------//
#endif 

