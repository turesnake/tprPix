/*
 * ========================= esrc_ui.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_UI_H_
#define _TPR_ESRC_UI_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <functional> 
#include <unordered_map>
#include <unordered_set> 


//-------------------- Engine --------------------//
#include "UIObj.h"
#include "esrc_funcTypes.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


//--- mem ---//
inline std::unordered_map<uiObjId_t, UIObj> memUIs {}; //- 所有 ui实例 实际存储区。

                                    
inline std::unordered_set<uiObjId_t> uiIds_active   {}; 
                //-- 这个 好像没有被用到 ？？？



void foreach_memUIs( F_UIObjId_GOPTR _fp );
void foreach_uiIds_active( F_UIObjId_GOPTR _fp );


uiObjId_t insert_new_ui();



inline UIObj *get_memUIPtr( uiObjId_t _uiObjid ){
        assert( memUIs.find(_uiObjid) != memUIs.end() );//- tmp
    return  static_cast<UIObj*>( &(memUIs.at(_uiObjid)) );
}




}//---------------------- namespace: esrc -------------------------//
#endif

