/*
 * ========================= esrc_ui.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_UI_H
#define TPR_ESRC_UI_H

//-------------------- CPP --------------------//
#include <functional> 
#include <unordered_map>
#include <unordered_set> 


//-------------------- Engine --------------------//
#include "UIObj.h"
#include "esrc_funcTypes.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_uis();

std::unordered_map<uiObjId_t, UIObj> &get_memUIs();
std::unordered_set<uiObjId_t> &get_uiIds_active();


void foreach_memUIs( F_UIObjId_GOPTR fp_ );
void foreach_uiIds_active( F_UIObjId_GOPTR fp_ );


uiObjId_t insert_new_ui();

UIObj *get_memUIPtr( uiObjId_t uiObjid_ );


}//---------------------- namespace: esrc -------------------------//
#endif

