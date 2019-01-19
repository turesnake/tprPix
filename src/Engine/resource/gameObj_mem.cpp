/*
 * ========================= gameObj_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.22
 *                                        修改 -- 2018.12.22
 * ----------------------------------------------------------
 *  GameObj 在 内存中的 管理
 * ----------------------------
 */
#include "srcs_engine.h" //- 所有资源

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr


//-------------------- CPP --------------------//
#include <vector>


using std::vector;


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                  insert_new_gameObj
 * -----------------------------------------------------------
 * -- 创建1个 go实例，并为其分配新 goid. 然后存入 memGameObjs 容器中
 * -- 不能用在 从 硬盘读出的 go数据上
 * -- return：
 *     新实例的 id 号
 */
goid_t insert_new_gameObj(){

    // ***| INSERT FIRST, INIT LATER  |***
    GameObj  tmp_go {};
    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    tmp_go.id = goid; //-- MUST --
    memGameObjs.insert({ goid, tmp_go }); //- copy
    memGameObjs.at(goid).init(); //- MUST --
    return goid;
}


/* ===========================================================
 *                    realloc_active_goes
 * -----------------------------------------------------------
 * -- 检测 激活go组，
 * -- 将 离开 激活圈的 go，移动到 未激活组
 */
void realloc_active_goes(){

    vector<goid_t> container; //- tmp, 容纳 命中的id

    GameObj *goPtr;

    glm::vec2 v;
    float distance;
    float range = 1600.0f; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : goids_active ){

        goPtr = esrc::find_memGameObjs( id );

        v = camera.get_camera2DPos() - goPtr->move.get_currentFPos();
        distance = v.x * v.x + v.y * v.y;

        //-- 将离开 激活圈的 go 移动到 激活组 --
        if( distance > range ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        goids_inactive.insert( i );
        goids_active.erase( i );
    }
}


/* ===========================================================
 *                    realloc_inactive_goes
 * -----------------------------------------------------------
 * -- 检测 未激活go组，
 * -- 将 进入 激活圈的 go，移动到 激活组
 */
void realloc_inactive_goes(){

    vector<goid_t> container; //- tmp, 容纳 命中的id

    GameObj *goPtr;

    glm::vec2 v;
    float distance;
    float range = 1600.0f; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : goids_inactive ){

        goPtr = esrc::find_memGameObjs( id );
        
        v = camera.get_camera2DPos() - goPtr->move.get_currentFPos();
        distance = v.x * v.x + v.y * v.y;

        //-- 将进入 激活圈的 go 移动到 激活组 --
        if( distance <= range ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        goids_active.insert( i );
        goids_inactive.erase( i );
    }
}







}//---------------------- namespace: esrc -------------------------//

