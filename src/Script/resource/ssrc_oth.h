/*
 * ======================== ssrc_oth.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SSRC_OTH_H
#define TPR_SSRC_OTH_H

//-------------------- CPP --------------------//
#include <functional> 

//-------------------- Engine --------------------//
#include "GameObjType.h"
#include "dyParams.h"

class GameObj;
class UIObj;

//-- map自动生成器 使用的 uiInit函数 ---
using F_GO_INIT = std::function<void(GameObj&, const DyParam &dyParams_)>;
using F_UI_INIT = std::function<void(UIObj*, const glm::vec2 &)>;


#endif 

