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

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"

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

    cout << "esrc_gameObjs:"
        << "\n    gameObjs: " << go_inn::gameObjs.size() 
        << "\n    gos_active: " << go_inn::goids_active.size()
        << "\n    gos_inactive: " << go_inn::goids_inactive.size() 
        << endl;
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

/* ===========================================================
 *                  get_goRef
 * -----------------------------------------------------------
 * -- 返回 go实例 引用
 * -- 通过此接口，外部只有权 读取改写 go实例，无权删除 go实例，无法长期持有 go实例指针
 */
GameObj &get_goRef( goid_t id_, const std::string str_ ){

        //-- debug 
        if( go_inn::gameObjs.find(id_) == go_inn::gameObjs.end() ){
            cout << "ERROR:get_goRef(): " << str_
                << "\n    goid = " << id_ 
                << endl;
        }

        tprAssert( go_inn::gameObjs.find(id_) != go_inn::gameObjs.end() );//- tmp
    return *(go_inn::gameObjs.at(id_));
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
    auto outPair = go_inn::goids_active.insert( id_ );
    tprAssert( outPair.second );
}

void insert_2_goids_inactive( goid_t id_ ){
    auto outPair = go_inn::goids_inactive.insert( id_ );
    tprAssert( outPair.second );
}

/* ===========================================================
 *                  insert_new_regularGo
 * -----------------------------------------------------------
 * -- 创建1个 go实例，并为其分配新 goid. 然后存入 memGameObjs 容器中
 * -- 不能用在 从 硬盘读出的 go数据上
 * -- return：
 *     新实例的 id 号
 */
goid_t insert_new_regularGo( const glm::dvec2 &dpos_ ){
    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    auto outPair = go_inn::gameObjs.insert({ goid, GameObj::factory_for_regularGo( goid, dpos_ ) });
    tprAssert( outPair.second );
    return goid;
}

/* ===========================================================
 *                  insert_new_uiGo
 * -----------------------------------------------------------
 * -- 创建1个 uiGo实例，并为其分配新 goid. 然后存入 memGameObjs 容器中
 */
goid_t insert_new_uiGo( const glm::dvec2 &basePointProportion_,
                        const glm::dvec2 &offDPos_ ){

    goid_t goid = GameObj::id_manager.apply_a_u64_id();
    auto outPair = go_inn::gameObjs.insert({ goid, GameObj::factory_for_uiGo( goid, basePointProportion_, offDPos_ ) });
    tprAssert( outPair.second );
    return goid;
}




/* ===========================================================
 *                  insert_a_diskGo
 * -----------------------------------------------------------
 * -- 从 db 中读取一个 go数据，根据此数据，来重建一个 mem态 go 实例
 * -- 为其分配新 goid. 然后存入 memGameObjs 容器中
 */
void insert_a_diskGo( goid_t goid_, const glm::dvec2 &dpos_ ){
    auto outPair = go_inn::gameObjs.insert({ goid_, GameObj::factory_for_regularGo(goid_,dpos_) });
    tprAssert( outPair.second );
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
        GameObj &goRef = esrc::get_goRef(id, "realloc_active_goes");

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
        GameObj &goRef = esrc::get_goRef(id, "realloc_inactive_goes");

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


/* ===========================================================
 *                 signUp_newGO_to_chunk_and_mapEnt
 * -----------------------------------------------------------
 * -- 将 新建go 的 colliEnts 登记到所有对应的 mapent 上去。
 * 难点：
 *    有些身处 chunk边缘的 “临界go” 的 collient，可以位于 隔壁chunk
 *    而这个 隔壁 chunk，可能尚未创建。（此处会引发各种麻烦）
 *    目前解决方案：
 *      --- 新建go 跳过这个 collient 的登记工作
 *      --- 统计自己的 chunkeys,
 *      --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
 *      --- 将 本goid，记录到 主chunk goids 容器中
 * 
 * -- 目前仅用于 create_a_Go() 系列函数
 */
void signUp_newGO_to_chunk_and_mapEnt( GameObj &goRef_ ){

    //-------------------------------//
    // 如果不是 MajorGo, 彻底不参与 chunk 方面的登记操作
    // 比如 goCir 这种 Oth go
    // 这会让它们成为 “无法通过 chunk 收集并删除的 go”
    // 类似的还有，类似血条，伴随 活体go 移动的 worldUI-go
    // 它们该如何 释放，是个问题 ...
    //
    if( goRef_.family != GameObjFamily::Major ){
        return;
    }

    //------------------------------//
    // --- 记录 go.currentChunkKey
    // --- 统计自己的 chunkeys
    // --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
    // --- 将 本goid，记录到 主chunk goids 容器中
    //------------------------------//
    auto outPair = esrc::get_chunkPtr( goRef_.currentChunkKey );
    tprAssert( outPair.first == ChunkMemState::Active );
    Chunk &currentChunkRef = *(outPair.second);

    currentChunkRef.insert_2_goIds( goRef_.id ); //- always

    //------------------------------//
    size_t      chunkKeySize = goRef_.reCollect_chunkKeys();
    chunkKey_t  tmpChunkKey  {};

    auto colliType = goRef_.get_colliderType();
    if( colliType == ColliderType::Square ){

        //-- 目前暂时只支持 1*1mapent 尺寸的 人造物单元
        //   所以完全不用关心 chunk.edgeGoids 的问题
        //   最简设计：

        auto mpos = dpos_2_mpos( goRef_.get_dpos() );

        //---- 正式注册 go 到 mapents 上 -----
        auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
        tprAssert( mapEntPair.first == ChunkMemState::Active );
        mapEntPair.second->set_square_goid( goRef_.id, colliType );
        

    }else if( colliType == ColliderType::Circular ){

        if( chunkKeySize > 1 ){
            currentChunkRef.insert_2_edgeGoIds( goRef_.id );
        }
        //------------------------------//
        //  signUp each collient to mapEnt
        //------------------------------//
        
        const auto &currentSignINMapEntsRef = goRef_.get_collisionRef().get_currentSignINMapEntsRef_for_cirGo();
        tprAssert( !currentSignINMapEntsRef.empty() ); //- tmp
        for( const auto &mpos : currentSignINMapEntsRef ){

            tmpChunkKey = anyMPos_2_chunkKey( mpos );

            //-- 如果 colliEnt所在 chunk 尚未创建，表明此 go 为 “临界go”。
            // 此时显然不能去调用 esrc::getnc_memMapEntPtr(), 会出错。
            // 将会暂时 忽略掉这个 collient 的登记工作，
            // 这个工作，会等到 目标chunk 创建阶段，再补上: 
            // 在 signUp_nearby_chunks_edgeGo_2_mapEnt() 中
            auto chunkState = esrc::get_chunkMemState(tmpChunkKey);
            if( (chunkState==ChunkMemState::NotExist) || 
                (chunkState==ChunkMemState::WaitForCreate) ||
                (chunkState==ChunkMemState::OnCreating) ){
                continue;
            }

            //---- 正式注册 go 到 mapents 上 -----
            auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
            tprAssert( mapEntPair.first == ChunkMemState::Active );
            mapEntPair.second->insert_2_circular_goids( goRef_.id, colliType );
        }

    }else{
        // do nothing !
        return;
    }

}


}//---------------------- namespace: esrc -------------------------//

