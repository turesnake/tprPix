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

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
//#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
//#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr


//-------------------- CPP --------------------//
#include <vector>


using std::vector;


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                  insert_new_ui
 * -----------------------------------------------------------
 * -- 创建1个 go实例，并为其分配新 goid. 然后存入 ui 容器中
 * -- 不能用在 从 硬盘读出的 go数据上
 * -- return：
 *     新实例的 id 号
 */
goid_t insert_new_ui(){

    // ***| INSERT FIRST, INIT LATER  |***
    GameObj  tmp_go {};
    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    tmp_go.id = goid; //-- MUST --
        assert( esrc::memUIs.find(goid) == esrc::memUIs.end() );//- must not exist
    esrc::memUIs.insert({ goid, tmp_go }); //- copy
    esrc::memUIs.at(goid).init(); //- MUST --
    return goid;
}







}//---------------------- namespace: esrc -------------------------//

