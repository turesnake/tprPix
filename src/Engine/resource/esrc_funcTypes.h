/*
 * ===================== esrc_funcTypes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_FUNC_TYPES_H_
#define TPR_FUNC_TYPES_H_

//-------------------- CPP --------------------//
#include <functional> 

//-------------------- Engine --------------------//
#include "GameObjType.h"
#include "UIObjType.h"

class GameObj;
class UIObj;


namespace esrc {//------------------ namespace: esrc -------------------------//

using FUNC_V_V = std::function<void()>;
using F_GOID_GOPTR = std::function<void(goid_t,GameObj*)>;

using F_UIObjId_GOPTR = std::function<void(uiObjId_t,UIObj*)>;


}//---------------------- namespace: esrc -------------------------//
#endif 

