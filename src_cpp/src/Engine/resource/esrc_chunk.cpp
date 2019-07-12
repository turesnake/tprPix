/*
 * ========================= esrc_chunk.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "esrc_chunk.h"
//-------------------- CPP --------------------//
#include <unordered_map>
#include <memory>
#include <deque>
#include <unordered_set>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "chunkBuild.h"
#include "esrc_renderPool.h"

#include "tprDebug.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace chunk_inn {//------------ namespace: chunk_inn --------------//
    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, std::unique_ptr<Chunk>> chunks {};

    std::unique_ptr<ChunkCreateReleaseZone> chunkCreateReleaseZoneUPtr;

}//---------------- namespace: chunk_inn end --------------//

extern void chunkStates_debug();
void chunks_debug(){
    cout << "\nchunks.size() = " << chunk_inn::chunks.size() 
        << endl;
    chunkStates_debug();
}


void init_chunkCreateReleaseZone( const IntVec2 &playerMPos_ ){
    chunk_inn::chunkCreateReleaseZoneUPtr->init( playerMPos_ );
}

ChunkZone &get_chunkCreateZoneRef(){
    return chunk_inn::chunkCreateReleaseZoneUPtr->get_createZoneRef();
}

ChunkZone &get_chunkReleaseZoneRef(){
    return chunk_inn::chunkCreateReleaseZoneUPtr->get_releaseZoneRef();
}


//- only used for esrc_chunkMemState -
bool find_from_chunks( chunkKey_t chunkKey_ ){
    return (chunk_inn::chunks.find(chunkKey_)!=chunk_inn::chunks.end());
}



/* ===========================================================
 *                    init_chunks
 * -----------------------------------------------------------
 */
void init_chunks(){
    //chunk_inn::chunkCreateZoneUPtr = std::make_unique<ChunkZone>(3);
    //chunk_inn::chunkReleaseZoneUPtr = std::make_unique<ChunkZone>(7);
    chunk_inn::chunkCreateReleaseZoneUPtr = std::make_unique<ChunkCreateReleaseZone>( 3, 7 );
    //...
}

/* ===========================================================
 *             insert_and_init_new_chunk
 * -----------------------------------------------------------
 * 创建 chunk实例，放入 全局容器，且初始化它
 * 仅被 check_and_build_sections.cpp -> build_one_chunk() 调用
 */
Chunk &insert_and_init_new_chunk( chunkKey_t chunkKey_ ){

    auto chunkUPtr = std::make_unique<Chunk>();
    chunkUPtr->set_by_chunkKey(chunkKey_);
    chunkUPtr->init();

    chunk_inn::chunks.insert({ chunkKey_, std::move(chunkUPtr) });
    esrc::move_chunkKey_from_onCreating_2_active(chunkKey_);  
                            // Now, the chunkState is Active !!! 
    return *(chunk_inn::chunks.at(chunkKey_).get());
}


/* ===========================================================
 *             erase_from_chunks
 * -----------------------------------------------------------
 */
extern void erase_chunkKey_from_onReleasing( chunkKey_t chunkKey_ );
void erase_from_chunks( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnReleasing );
    tprAssert(chunk_inn::chunks.erase(chunkKey_)==1);
    esrc::erase_chunkKey_from_onReleasing(chunkKey_);
}



/* ===========================================================
 *          get_memMapEntRef_in_activeChunk     [严格版]
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 *    当目标 chunkMemState 不为 Active，直接报错
 */
MemMapEnt &get_memMapEntRef_in_activeChunk( const IntVec2 &anyMPos_ ){

    //-- 计算 目标 chunk 的 key --
    chunkKey_t    chunkKey = anyMPos_2_chunkKey( anyMPos_ );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( anyMPos_ );

    if( get_chunkMemState(chunkKey) != ChunkMemState::Active ){
        tprAssert(0);
    }

    return chunk_inn::chunks.at(chunkKey)->getnc_mapEntRef_by_lMPosOff( lMPosOff );
}



/* ===========================================================
 *                 get_chunkRef
 * -----------------------------------------------------------
 */
Chunk &get_chunkRef( chunkKey_t key_ ){
        tprAssert( get_chunkMemState(key_) == ChunkMemState::Active );
    return *(chunk_inn::chunks.at(key_).get());
}

/* ===========================================================
 *                 render_chunks
 * -----------------------------------------------------------
 * 每一渲染帧，都要将所有 Active 态的 chunks， 重新存入 renderPool_meshs
 * 从而给它们做一次排序。
 */
extern const std::unordered_set<chunkKey_t> &get_chunkKeys_active();
void render_chunks(){

    const auto &activeKeys = esrc::get_chunkKeys_active();
    for( auto &key : activeKeys ){
        
        auto &chunkRef = *(chunk_inn::chunks.at(key).get());
        const auto &meshRef = chunkRef.get_mesh();
        chunkRef.refresh_translate_auto(); //-- MUST !!!
        insert_2_renderPool_meshs( meshRef.get_render_z(),
                                    const_cast<Mesh*>(&meshRef) );
    }
}


}//---------------------- namespace: esrc -------------------------//

