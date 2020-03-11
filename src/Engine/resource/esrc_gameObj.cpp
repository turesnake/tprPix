/*
 * ====================== esrc_gameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  GameObj 在 内存中的 管理
 * ----------------------------
 */
#include "pch.h"
#include "esrc_gameObj.h"
#include "esrc_camera.h"

//-------------------- Engine --------------------//
#include "esrc_chunk.h"
#include "esrc_state.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace go_inn {//-------- namespace: go_inn --------------//
    // main thread only
    std::unordered_map<goid_t, std::shared_ptr<GameObj>> gameObjs {};  //- 所有载入内存的 go实例 实际存储区。
       
    std::unordered_set<goid_t> goids_active   {}; //- 激活组 (身处 激活圈 之内)
    std::unordered_set<goid_t> goids_inactive {}; //- 未激活组 (身处 激活圈 之外)
                            // 这组概念可能会被改动，比如，配合新的 GoMemState 机制
                            // ...
                            // ------
                            // 在 go类 的适用范围被扩充后，部分go 不需要这么复杂的功能
                            // 比如，也许只有 MajorGo 需要，剩余的 go，可以彻底不参与 active 部分的活动
                            // 未实现... 
                            // ---
                            // 在目前实现中，只有 active 的go 才会被渲染....
                            
    F_void  goSpeciesIds_SignUp  {nullptr}; //- goSpeciesIds 注册函数对象

    //double activeRange { 2048.0 * 2048.0 }; // （1 chunk 尺寸）这个尺寸已经非常大了
    double activeRange { 2048.0 * 2048.0 * 0.7 };
                        // 这个尺寸很不错，缩小比例建议设置在 (0.45, 0.8) 之间
                        // 0.45: 画面边缘已经出现破绽
                        // 0.65  active go 数量维持在 300左右，（绝大部分为 地景

    //double activeRange { 2048.0 * 2048.0 * 4 };


}//------------- namespace: go_inn end --------------//


void init_gameObjs(){
    go_inn::gameObjs.reserve(10000);
    go_inn::goids_active.reserve(10000);
    go_inn::goids_inactive.reserve(10000);
    esrc::insertState("gameObj");
}

void debug_for_gameObjs()noexcept{

    tprDebug::console( 
        "esrc_gameObjs:" \
        "\n    gameObjs: {0}" \
        "\n    gos_active: {1}" \
        "\n    gos_inactive: {2}",
        go_inn::gameObjs.size() ,
        go_inn::goids_active.size(),
        go_inn::goids_inactive.size() 
    );
}


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



std::optional<GameObj*> get_goPtr(goid_t id_)noexcept{
    if( go_inn::gameObjs.find(id_) == go_inn::gameObjs.end() ){
        return std::nullopt;
    }
    return go_inn::gameObjs.at(id_).get();
}



bool is_go_active(goid_t id_  ){
    return (go_inn::gameObjs.find(id_) != go_inn::gameObjs.end());
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

/*
GameObj *get_goRawPtr( goid_t id_ ){
        tprAssert( go_inn::gameObjs.find(id_) != go_inn::gameObjs.end() );//- tmp
    return go_inn::gameObjs.at(id_).get();
}
*/

std::unordered_set<goid_t> &get_goids_active(){
    return go_inn::goids_active;
}

std::unordered_set<goid_t> &get_goids_inactive(){
    return go_inn::goids_inactive;
}

void insert_2_goids_active( goid_t id_ ){
    auto [insertIt, insertBool] = go_inn::goids_active.insert( id_ );
    tprAssert( insertBool );
}

void insert_2_goids_inactive( goid_t id_ ){
    auto [insertIt, insertBool] = go_inn::goids_inactive.insert( id_ );
    tprAssert( insertBool );
}

/* ===========================================================
 *                  insert_new_regularGo
 * -----------------------------------------------------------
 * -- 创建1个 go实例，并为其分配新 goid. 然后存入 memGameObjs 容器中
 * -- 不能用在 从 硬盘读出的 go数据上
 * -- return：
 *     新实例的 goid 号
 */
goid_t insert_new_regularGo( const glm::dvec2 &dpos_, size_t goUWeight_ ){
    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    auto [insertIt, insertBool] = go_inn::gameObjs.emplace( goid, GameObj::factory_for_regularGo( goid, dpos_, goUWeight_ ) );
    tprAssert( insertBool );
    return goid;
}

/* ===========================================================
 *                  insert_new_uiGo
 * -----------------------------------------------------------
 * -- 创建1个 uiGo实例，并为其分配新 goid. 然后存入 memGameObjs 容器中
 */
goid_t insert_new_uiGo( const glm::dvec2 &basePointProportion_,
                        const glm::dvec2 &offDPos_,
                        size_t goUWeight_ ){

    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    auto [insertIt, insertBool] = go_inn::gameObjs.emplace( goid, GameObj::factory_for_uiGo( goid, basePointProportion_, offDPos_, goUWeight_ ) );
    tprAssert( insertBool );
    return goid;
}




/* ===========================================================
 *                  insert_a_diskGo
 * -----------------------------------------------------------
 * -- 从 db 中读取一个 go数据，根据此数据，来重建一个 mem态 go 实例
 * -- 为其分配新 goid. 然后存入 memGameObjs 容器中
 */
void insert_a_diskGo( goid_t goid_, const glm::dvec2 &dpos_, size_t goUWeight_ ){
    auto [insertIt, insertBool] = go_inn::gameObjs.emplace( goid_, GameObj::factory_for_regularGo(goid_, dpos_, goUWeight_) );
    tprAssert( insertBool );
}

/* ===========================================================
 *                    realloc_active_goes
 * -----------------------------------------------------------
 * -- 检测 激活go组，
 * -- 将 离开 激活圈的 go，移动到 未激活组
 */
void realloc_active_goes(){

    std::vector<goid_t> container {}; //- tmp, 容纳 命中的id
    container.reserve(10000);
                    // 此容器也许可以被做成 全局容器

    glm::dvec2 v    {};
    double distance {};

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : go_inn::goids_active ){
        // get go ref
        auto goOpt = esrc::get_goPtr( id );
        tprAssert( goOpt.has_value() );
        GameObj &goRef = *goOpt.value();

        v = get_camera().get_currentDPos() - goRef.get_dpos();

        distance = (v.x * v.x) + (v.y * v.y);
        //-- 将离开 激活圈的 go 移动到 激活组 --
        if( distance > go_inn::activeRange ){
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
    container.reserve(10000);
                    // 此容器也许可以被做成 全局容器
                    
    glm::dvec2 v    {};
    double distance {};

    //-- 将 符合条件的 goid 先放到一个 vector 容器中 --
    for( auto id : go_inn::goids_inactive ){
        // get go ref
        auto goOpt = esrc::get_goPtr( id );
        tprAssert( goOpt.has_value() );
        GameObj &goRef = *goOpt.value();

        v = get_camera().get_currentDPos() - goRef.get_dpos();

        distance = (v.x * v.x) + (v.y * v.y);
        //-- 将进入 激活圈的 go 移动到 激活组 --
        if( distance <= go_inn::activeRange ){
            container.push_back( id );
        }
    }

    //-- 正式移动 这些 goid --
    for( auto i : container ){
        go_inn::goids_active.insert( i );
        go_inn::goids_inactive.erase( i );
    }
}


// 检查 go在位移后，是否跨越 chunk
// 如果发生跨越，修改 go chunk登记信息
// 仅适用于 WorldUI-go 
void refresh_worldUIGo_chunkSignUpData( GameObj &goRef_, const glm::dvec2 &moveVec_ ){

    tprAssert( goRef_.family == GameObjFamily::WorldUI );
    //---
    chunkKey_t newChunkKey = anyDPos_2_chunkKey( goRef_.get_dpos() + moveVec_ );
    if( newChunkKey != goRef_.currentChunkKey ){
        // 确实需要 更新 登记信息

        auto [ newChunkMemState, newChunkPtr ] = esrc::get_chunkPtr( newChunkKey );
        tprAssert( newChunkMemState == ChunkMemState::Active ); // MUST

        auto [ oldChunkMemState, oldChunkPtr ] = esrc::get_chunkPtr( goRef_.currentChunkKey );
        tprAssert( oldChunkMemState == ChunkMemState::Active );  // MUST

        size_t eraseNum = oldChunkPtr->erase_from_goIds( goRef_.goid );
        tprAssert( eraseNum == 1 );
        oldChunkPtr->erase_from_edgeGoIds( goRef_.goid ); // maybe 
        //---
        goRef_.currentChunkKey = newChunkKey;
        newChunkPtr->insert_2_goIds( goRef_.goid );
    }
}




}//---------------------- namespace: esrc -------------------------//

