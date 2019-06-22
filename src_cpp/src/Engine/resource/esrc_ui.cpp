/*
 * ======================= esrc_ui.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  ui 在 内存中的 管理
 * ----------------------------
 */
#include "esrc_ui.h"
#include "esrc_camera.h"

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"


using std::vector;


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace ui_inn {//-------- namespace: ui_inn --------------//

    std::unordered_map<uiObjId_t, UIObj> memUIs {}; //- 所有 ui实例 实际存储区。

    std::unordered_set<uiObjId_t> uiIds_active  {}; 
                //-- 这个 好像没有被用到 ？？？

}//------------- namespace: ui_inn end --------------//


UIObj *get_memUIPtr( uiObjId_t _uiObjid ){
        tprAssert( ui_inn::memUIs.find(_uiObjid) != ui_inn::memUIs.end() );//- tmp
    return  &(ui_inn::memUIs.at(_uiObjid));
}


std::unordered_map<uiObjId_t, UIObj> &get_memUIs(){
    return ui_inn::memUIs;
}

std::unordered_set<uiObjId_t> &get_uiIds_active(){
    return ui_inn::uiIds_active;
}



/* ===========================================================
 *                  insert_new_ui
 * -----------------------------------------------------------
 * -- 创建1个 uiObj实例，并为其分配新 uiObjId. 然后存入 ui 容器中
 * -- return：
 *     新实例的 id 号
 */
uiObjId_t insert_new_ui(){

    // ***| INSERT FIRST, INIT LATER  |***
    UIObj  uiObj {};
    uiObjId_t uiGoId = UIObj::id_manager.apply_a_u32_id();
    uiObj.uiObjId = uiGoId; //-- MUST --
        tprAssert( ui_inn::memUIs.find(uiGoId) == ui_inn::memUIs.end() );//- must not exist
    ui_inn::memUIs.insert({ uiGoId, uiObj }); //- copy
    ui_inn::memUIs.at(uiGoId).init(); //- MUST --
    return uiGoId;
}


}//---------------------- namespace: esrc -------------------------//

