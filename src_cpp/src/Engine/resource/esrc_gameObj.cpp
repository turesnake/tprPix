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
#include "esrc_chunk.h"
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


/* ===========================================================
 *                 signUp_newGO_to_mapEnt
 * -----------------------------------------------------------
 * -- 将 新建go 的 collients 登记到所有对应的 mapent 上去。
 * 难点：
 *    有些身处 chunk边缘的 “临界go” 的 collient，可以位于 隔壁chunk
 *    而这个 隔壁 chunk，可能尚未创建。（此处会引发各种麻烦）
 *    目前解决方案：
 *      --- 新建go 跳过这个 collient 的登记工作
 *      --- 统计自己的 chunkeys,
 *      --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
 *      --- 将 本goid，记录到 主chunk goids 容器中
 */
void signUp_newGO_to_mapEnt( GameObj &goRef_ ){

    //------------------------------//
    // --- 记录 go.currentChunkKey
    // --- 统计自己的 chunkeys
    // --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
    // --- 将 本goid，记录到 主chunk goids 容器中
    //------------------------------//
    Chunk &currentChunkRef = esrc::get_chunkRef( goRef_.currentChunkKey );

    goRef_.reCollect_chunkKeys();
    if( goRef_.get_chunkKeysRef().size() > 1 ){
        currentChunkRef.insert_2_edgeGoIds( goRef_.id );
    }
    currentChunkRef.insert_2_goIds( goRef_.id ); //- always

    //------------------------------//
    //    only check rootGoMesh
    //------------------------------//
    if( goRef_.get_goMeshRef("root").isCollide == false ){
        return;
    }

    //------------------------------//
    //  signUp each collient to mapEnt
    //------------------------------//
    IntVec2     colliEntMPos {}; //- each collient mcpos [world-abs-pos]
    chunkKey_t  tmpChunkKey  {}; //- each collient current chunkKey

    const ColliEntHead &cehRef = goRef_.get_rootColliEntHeadRef();
    IntVec2  cesMPos = goRef_.get_rootCES_leftBottom_MPos(); //- rootCES left-bottom mcpos [world-abs-pos]

    const ColliEntSet &cesRef = esrc::get_colliEntSetRef( cehRef.colliEntSetIdx );
    for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in rootCES

        colliEntMPos = i.get_mpos() + cesMPos;
        tmpChunkKey = anyMPos_2_chunkKey( colliEntMPos );

        //-- 如果 colliEnt所在 chunk 尚未创建，表明此 go 为 “临界go”。
        // 此时显然不能去调用 esrc::get_memMapEntRef(), 会出错。
        // 将会暂时 忽略掉这个 collient 的登记工作，
        // 这个工作，会等到 目标chunk 创建阶段，再补上... 
        auto chunkState = esrc::get_chunkMemState(tmpChunkKey);
        if( (chunkState==ChunkMemState::NotExist) || (chunkState==ChunkMemState::OnCreating) ){
            continue;
        }

        //---- 正式注册 collient 到 mapents 上 -----
        auto &mapEntRef = esrc::get_memMapEntRef( colliEntMPos ); //- 目标 mapent 访问权 --
        //-- 并不检测 当前 mapent 中是否有 重合的 go。而是直接 将数据 存入 mapent
        mapEntRef.insert_2_major_gos(   goRef_.id, 
                                        cehRef.lGoAltiRange,
                                        cehRef.isCarryAffect );
    }
}


}//---------------------- namespace: esrc -------------------------//

