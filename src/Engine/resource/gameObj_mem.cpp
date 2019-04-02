/*
 * ========================= gameObj_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
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
        assert( esrc::memGameObjs.find(goid) == esrc::memGameObjs.end() );//- must not exist
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

        goPtr = esrc::get_memGameObjs( id );

        v = camera.get_camera2DFPos() - goPtr->goPos.get_currentFPos();
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

        goPtr = esrc::get_memGameObjs( id );
        
        v = camera.get_camera2DFPos() - goPtr->goPos.get_currentFPos();
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


/* ===========================================================
 *                 signUp_newGO_to_mapEnt
 * -----------------------------------------------------------
 * -- 
 */
void signUp_newGO_to_mapEnt( GameObj *_goPtr ){

    IntVec2   currentPPos = _goPtr->goPos.get_currentPPos();  //-- 直指 current rootAnchor 检测用
    MapCoord  cesMCPos;      //- 每个 ces左下角的 mcpos （世界绝对pos）
    //MapCoord  colliEntMCPos; //- adds/dels 中，每个ent 的 mcpos （世界绝对pos）
    MemMapEnt *mapEntPtr;    //- 目标 mapent

    //------------------------------//
    //  遍历每个  go.goMesh
    //  遍历每个  ces
    //------------------------------//
    auto ipair = _goPtr->goMeshs.begin();
    for( ; ipair!=_goPtr->goMeshs.end(); ipair++ ){ //- each gomesh
        GameObjMesh &meshRef = ipair->second;

        //-- 未开启碰撞检测的 mesh 可以直接跳过 --
        if( meshRef.isCollide == false ){
            continue;
        }

        for( auto &ceh : meshRef.get_currentFramePos().get_colliEntHeads() ){ //-- each ceh
 
            cesMCPos.set_by_ppos( currentPPos + ceh.pposOff_fromRootAnchor );
            ColliEntSet &cesRef = esrc::colliEntSets.at( ceh.colliEntSetIdx ); //- get ces ref

            for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in target_ces

                //colliEntMCPos = i + cesMCPos;
                //-- 这就是 每一个 ces.collient 的 mcpos

                mapEntPtr = esrc::get_memMapEntPtr( i + cesMCPos ); //- 目标 mapent 访问权 --

                //-- 并不检测 当前 mapent 中是否有 重合的 go。而是直接 将数据 存入 mapent
                mapEntPtr->major_gos.insert({ _goPtr->id, 
                                              MajorGO_in_MapEnt{ ceh.lAltiRange, ceh.isCarryAffect } });
            }
        }
    }
}





}//---------------------- namespace: esrc -------------------------//

