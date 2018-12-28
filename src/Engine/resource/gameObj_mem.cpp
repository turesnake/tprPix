/*
 * ========================= gameObj_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.22
 *                                        修改 -- 2018.12.22
 * ----------------------------------------------------------
 *  GameObj 在 内存中的 管理
 * ----------------------------
 */
#include "Engine/resource/srcs_manager.h" //- 所有资源

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


namespace src{ //------------------ namespace: src -------------------------//


/* ===========================================================
 *                    realloc_active_goes
 * -----------------------------------------------------------
 * -- 检测 激活go组，
 * -- 将 离开 激活圈的 go，移动到 未激活组
 */
void realloc_active_goes(){

    vector<goid_t> container; //- tmp, 容纳 命中的id

    GameObj *goptr;

    glm::vec2 v;
    float distance;
    float range = 1600.0f; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : src::goids_active ){

        goptr = &(src::memGameObjs.at( id ));

        v = src::camera.get_camera2DPos() - goptr->currentPos;
        distance = v.x * v.x + v.y * v.y;

        //-- 将离开 激活圈的 go 移动到 激活组 --
        if( distance > range ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        src::goids_inactive.insert( i );
        src::goids_active.erase( i );
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

    GameObj *goptr;

    glm::vec2 v;
    float distance;
    float range = 1600.0f; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : src::goids_inactive ){

        goptr = &(src::memGameObjs.at( id ));

        v = src::camera.get_camera2DPos() - goptr->currentPos;
        distance = v.x * v.x + v.y * v.y;

        //-- 将进入 激活圈的 go 移动到 激活组 --
        if( distance <= range ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        src::goids_active.insert( i );
        src::goids_inactive.erase( i );
    }
}





}//---------------------- namespace: src -------------------------//

