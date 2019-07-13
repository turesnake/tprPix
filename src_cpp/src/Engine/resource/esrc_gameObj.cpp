/*
 * ====================== esrc_gameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  GameObj 在 内存中的 管理
 * ----------------------------
 */
#include "esrc_gameObj.h"
#include "esrc_camera.h"
#include "esrc_colliEntSet.h"

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace go_inn {//-------- namespace: go_inn --------------//

    std::unordered_map<goid_t, std::shared_ptr<GameObj>> gameObjs {};  //- 所有载入内存的 go实例 实际存储区。
       
    std::unordered_set<goid_t> goids_active   {}; //- 激活组 (身处 激活圈 之内)
    std::unordered_set<goid_t> goids_inactive {}; //- 未激活组 (身处 激活圈 之外)

    FUNC_V_V  goSpecIds_SignUp  {nullptr}; //- goSpecIds 注册函数对象

}//------------- namespace: go_inn end --------------//

/* ===========================================================
 *                  get_goWPtr
 * -----------------------------------------------------------
 * -- 只有那些 可以 共享 go资源 所有权 的代码，才能调用此函数
 * -- 目前禁止被调用
 */
std::weak_ptr<GameObj> get_goWPtr( goid_t id_ ){
        tprAssert( go_inn::gameObjs.find(id_) != go_inn::gameObjs.end() );//- tmp
    return  std::weak_ptr<GameObj>( go_inn::gameObjs.at(id_) );
}

/* ===========================================================
 *                  get_goRef
 * -----------------------------------------------------------
 * -- 返回 go实例 引用
 * -- 通过此接口，外部只有权 读取改写 go实例，无权删除 go实例，无关长期持有 go实例指针
 */
GameObj &get_goRef( goid_t id_ ){
        tprAssert( go_inn::gameObjs.find(id_) != go_inn::gameObjs.end() );//- tmp
    return *(go_inn::gameObjs.at(id_).get());
}

/* ===========================================================
 *                  erase_the_go
 * -----------------------------------------------------------
 */
void erase_the_go( goid_t id_ ){
        
    go_inn::goids_active.erase(id_);
    go_inn::goids_inactive.erase(id_);
    size_t eraseNum = go_inn::gameObjs.erase(id_);
    tprAssert( eraseNum == 1 );
}





/* ===========================================================
 *                  get_goRawPtr
 * -----------------------------------------------------------
 * -- 返回 go实例 裸指针
 * -- 通过此接口，外部只有权 读取改写 go实例，无权删除 go实例，无关长期持有 go实例指针
 */
GameObj *get_goRawPtr( goid_t id_ ){
        tprAssert( go_inn::gameObjs.find(id_) != go_inn::gameObjs.end() );//- tmp
    return go_inn::gameObjs.at(id_).get();
}

std::unordered_set<goid_t> &get_goids_active(){
    return go_inn::goids_active;
}

std::unordered_set<goid_t> &get_goids_inactive(){
    return go_inn::goids_inactive;
}

void insert_2_goids_active( goid_t id_ ){
        tprAssert( go_inn::goids_active.find(id_) == go_inn::goids_active.end() );//- tmp
    go_inn::goids_active.insert( id_ );
}

/* ===========================================================
 *                  insert_new_gameObj
 * -----------------------------------------------------------
 * -- 创建1个 go实例，并为其分配新 goid. 然后存入 memGameObjs 容器中
 * -- 不能用在 从 硬盘读出的 go数据上
 * -- return：
 *     新实例的 id 号
 */
goid_t insert_new_gameObj( const IntVec2 &mpos_ ){

    goid_t goid = GameObj::id_manager.apply_a_u64_id();
        tprAssert( go_inn::gameObjs.find(goid) == go_inn::gameObjs.end() );//- must not exist        
    go_inn::gameObjs.insert({ goid, GameObj::factory( goid, mpos_ ) });
    return goid;
}

/* ===========================================================
 *                  insert_a_disk_gameObj
 * -----------------------------------------------------------
 * -- 从 db 中读取一个 go数据，根据此数据，来重建一个 mem态 go 实例
 * -- 为其分配新 goid. 然后存入 memGameObjs 容器中
 */
void insert_a_disk_gameObj( goid_t goid_, const IntVec2 &mpos_ ){

        tprAssert( go_inn::gameObjs.find(goid_) == go_inn::gameObjs.end() );//- must not exist
    go_inn::gameObjs.insert({ goid_, GameObj::factory(goid_, mpos_) });
}

/* ===========================================================
 *                    realloc_active_goes
 * -----------------------------------------------------------
 * -- 检测 激活go组，
 * -- 将 离开 激活圈的 go，移动到 未激活组
 */
void realloc_active_goes(){

    std::vector<goid_t> container {}; //- tmp, 容纳 命中的id

    glm::dvec2 v    {};
    double distance {};
    double range    { 1600.0 }; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : go_inn::goids_active ){
        GameObj &goRef = esrc::get_goRef(id);

        v = get_camera().get_camera2DDPos() - goRef.goPos.get_currentDPos();
        distance = v.x * v.x + v.y * v.y;
        //-- 将离开 激活圈的 go 移动到 激活组 --
        if( distance > range ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        go_inn::goids_inactive.insert( i );
        go_inn::goids_active.erase( i );
    }
}

/* ===========================================================
 *                    realloc_inactive_goes
 * -----------------------------------------------------------
 * -- 检测 未激活go组，
 * -- 将 进入 激活圈的 go，移动到 激活组
 */
void realloc_inactive_goes(){

    std::vector<goid_t> container {}; //- tmp, 容纳 命中的id

    glm::dvec2 v    {};
    double distance {};
    double range    { 1600.0 }; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : go_inn::goids_inactive ){
        GameObj &goRef = esrc::get_goRef(id);

        v = get_camera().get_camera2DDPos() - goRef.goPos.get_currentDPos();
        distance = v.x * v.x + v.y * v.y;
        //-- 将进入 激活圈的 go 移动到 激活组 --
        if( distance <= range ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        go_inn::goids_active.insert( i );
        go_inn::goids_inactive.erase( i );
    }
}


}//---------------------- namespace: esrc -------------------------//

