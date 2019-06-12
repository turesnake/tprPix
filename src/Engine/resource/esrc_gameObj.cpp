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

//-------------------- Engine --------------------//
#include "tprAssert.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace go_inn {//-------- namespace: go_inn --------------//

    std::unordered_map<goid_t, GameObj> memGameObjs {}; //- 所有载入内存的 go实例 实际存储区。
                                    
    std::unordered_set<goid_t> goids_active   {}; //- 激活组 (身处 激活圈 之内)
    std::unordered_set<goid_t> goids_inactive {}; //- 未激活组 (身处 激活圈 之外)

    FUNC_V_V  goSpecIds_SignUp  {nullptr}; //- goSpecIds 注册函数对象

}//------------- namespace: go_inn end --------------//


std::unordered_map<goid_t, GameObj> &get_memGameObjs(){
    return go_inn::memGameObjs;
}


std::unordered_set<goid_t> &get_goids_active(){
    return go_inn::goids_active;
}

std::unordered_set<goid_t> &get_goids_inactive(){
    return go_inn::goids_inactive;
}


GameObj *get_memGameObjPtr( goid_t _goid ){
        tprAssert( go_inn::memGameObjs.find(_goid) != go_inn::memGameObjs.end() );//- tmp
    return  &(go_inn::memGameObjs.at(_goid));
}


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
        tprAssert( go_inn::memGameObjs.find(goid) == go_inn::memGameObjs.end() );//- must not exist
    go_inn::memGameObjs.insert({ goid, tmp_go }); //- copy
    go_inn::memGameObjs.at(goid).init(); //- MUST --
    return goid;
}


/* ===========================================================
 *                  insert_a_disk_gameObj
 * -----------------------------------------------------------
 * -- 从 db 中读取一个 go数据，根据此数据，来重建一个 mem态 go 实例
 * -- 为其分配新 goid. 然后存入 memGameObjs 容器中
 */
void insert_a_disk_gameObj( goid_t _goid ){
    // ***| INSERT FIRST, INIT LATER  |***
    GameObj  tmp_go {};
    tmp_go.id = _goid; //-- MUST --
        tprAssert( go_inn::memGameObjs.find(_goid) == go_inn::memGameObjs.end() );//- must not exist
    go_inn::memGameObjs.insert({ _goid, tmp_go }); //- copy
    go_inn::memGameObjs.at(_goid).init(); //- MUST --
}



/* ===========================================================
 *                    realloc_active_goes
 * -----------------------------------------------------------
 * -- 检测 激活go组，
 * -- 将 离开 激活圈的 go，移动到 未激活组
 */
void realloc_active_goes(){

    std::vector<goid_t> container {}; //- tmp, 容纳 命中的id

    GameObj *goPtr {};

    glm::vec2 v    {};
    float distance {};
    float range    { 1600.0f }; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : go_inn::goids_active ){
        goPtr = esrc::get_memGameObjPtr( id );
        v = get_camera().get_camera2DFPos() - goPtr->goPos.get_currentFPos();
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

    GameObj *goPtr {};

    glm::vec2 v    {};
    float distance {};
    float range    { 1600.0f }; //- 激活圈 半径的平方

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : go_inn::goids_inactive ){
        goPtr = esrc::get_memGameObjPtr( id );
        v = get_camera().get_camera2DFPos() - goPtr->goPos.get_currentFPos();
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
 * 
 *  ----- 存在的问题 -----
 */
void signUp_newGO_to_mapEnt( GameObj *_goPtr ){

    IntVec2    currentMPos = _goPtr->goPos.get_currentMPos();
    MapCoord   cesMCPos      {}; //- 每个 ces左下角的 mcpos （世界绝对pos）
    MapCoord   colliEntMCPos {}; //- 每个 collient 的 mcpos （世界绝对pos）
    MemMapEnt  *mapEntPtr    {}; //- 目标 mapent
    chunkKey_t  tmpChunkKey  {}; //- 每个 collient 当前所在的 chunk key

    //------------------------------//
    // --- 记录 go.currentChunkKey
    // --- 统计自己的 chunkeys
    // --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
    // --- 将 本goid，记录到 主chunk goids 容器中
    //------------------------------//
    _goPtr->currentChunkKey = anyMPos_2_chunkKey( currentMPos );
    Chunk *currentChunkPtr = esrc::get_chunkPtr( _goPtr->currentChunkKey );
    _goPtr->reset_chunkKeys();
    if( _goPtr->get_chunkKeysRef().size() > 1 ){
        currentChunkPtr->insert_2_edgeGoIds( _goPtr->id );
    }
    currentChunkPtr->insert_2_goIds( _goPtr->id );

    //------------------------------//
    //  只有 rootGoMesh 会被记录到 mapent 上
    //  遍历其每个 ces
    //------------------------------//
    if( _goPtr->goMeshs.at("root").isCollide == false ){
        return;
    }

    const ColliEntHead *cehPtr = _goPtr->get_rootColliEntHeadPtr();

    cesMCPos.set_by_mpos( currentMPos - cehPtr->mposOff_from_cesLB_2_centerMPos );

    const ColliEntSet &cesRef = esrc::get_colliEntSetRef( cehPtr->colliEntSetIdx ); //- get ces ref

    for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in target_ces

        colliEntMCPos = i + cesMCPos; //-- 这就是 每一个 ces.collient 的 mcpos
        tmpChunkKey = anyMPos_2_chunkKey( colliEntMCPos.get_mpos() );

        //-- 如果 collient所在 chunk 尚未创建，表明此 go 为 “临界go”。
        // 此时显然不能去调用 esrc::get_memMapEntPtr(), 会出错。
        // 将会忽略掉这个 collient 的登记工作，
        if( !esrc::find_from_chunks(tmpChunkKey) ){
            continue;
        }

        //---- 正式注册 collient 到 mapents 上 -----
        mapEntPtr = esrc::get_memMapEntPtr( colliEntMCPos ); //- 目标 mapent 访问权 --
        //-- 并不检测 当前 mapent 中是否有 重合的 go。而是直接 将数据 存入 mapent
        mapEntPtr->major_gos.insert({   _goPtr->id, 
                                        MajorGO_in_MapEnt{ cehPtr->lAltiRange, cehPtr->isCarryAffect } });
    }
}


}//---------------------- namespace: esrc -------------------------//

