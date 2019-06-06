/*
 * ========================= ui_mem.cpp ==========================
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
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"


using std::vector;


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace {//-------- namespace: --------------//

    std::unordered_map<uiObjId_t, UIObj> memUIs {}; //- 所有 ui实例 实际存储区。

                                    
    std::unordered_set<uiObjId_t> uiIds_active   {}; 
                //-- 这个 好像没有被用到 ？？？

}//------------- namespace: end --------------//


UIObj *get_memUIPtr( uiObjId_t _uiObjid ){
        tprAssert( memUIs.find(_uiObjid) != memUIs.end() );//- tmp
    return  &(memUIs.at(_uiObjid));
}


std::unordered_map<uiObjId_t, UIObj> &get_memUIs(){
    return memUIs;
}

std::unordered_set<uiObjId_t> &get_uiIds_active(){
    return uiIds_active;
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
        tprAssert( esrc::memUIs.find(uiGoId) == esrc::memUIs.end() );//- must not exist
    esrc::memUIs.insert({ uiGoId, uiObj }); //- copy
    esrc::memUIs.at(uiGoId).init(); //- MUST --
    return uiGoId;
}


}//---------------------- namespace: esrc -------------------------//

