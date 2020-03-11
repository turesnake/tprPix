/*
 * ========================= esrc_chunk.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_chunk.h"
//-------------------- CPP --------------------//
#include <deque>

//-------------------- Engine --------------------//
#include "chunkCreate.h"

#include "esrc_renderPool.h"
#include "esrc_state.h"
#include "esrc_player.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace chunk_inn {//------------ namespace: chunk_inn --------------//
    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, std::unique_ptr<Chunk>> chunks {};

    std::unique_ptr<ChunkCreateReleaseZone> chunkCreateReleaseZoneUPtr;

}//---------------- namespace: chunk_inn end --------------//

extern void chunkStates_debug();
extern void erase_chunkKey_from_onReleasing( chunkKey_t chunkKey_ )noexcept;
extern const std::unordered_set<chunkKey_t> &get_chunkKeys_active()noexcept;


void init_chunks(){
    chunk_inn::chunks.reserve(100);
    chunk_inn::chunkCreateReleaseZoneUPtr = std::make_unique<ChunkCreateReleaseZone>( 3, 7 );
                                                                // 也许应该改为 3,9
    //...
    esrc::insertState("chunk");
}


void chunks_debug(){
    tprDebug::console( "\nchunks.size() = {}", chunk_inn::chunks.size() );
    chunkStates_debug();
}


void init_chunkCreateReleaseZone( IntVec2 playerMPos_ ){
    chunk_inn::chunkCreateReleaseZoneUPtr->init( playerMPos_ );
}

ChunkCreateReleaseZone &get_chunkCreateReleaseZoneRef(){
    return *(chunk_inn::chunkCreateReleaseZoneUPtr);
}


/* 创建 chunk实例，放入 全局容器，且初始化它
 * 仅被 check_and_build_sections.cpp -> create_one_chunk() 调用
 */
Chunk &insert_and_init_new_chunk( chunkKey_t chunkKey_ ){
    auto [insertIt, insertBool] = chunk_inn::chunks.emplace( chunkKey_, std::make_unique<Chunk>( chunkKey_ ) );
    tprAssert( insertBool );
    Chunk &chunkRef = *(insertIt->second);

    esrc::move_chunkKey_from_onCreating_2_active(chunkKey_);  
                            // Now, the chunkState is Active !!! 
    return chunkRef;
}



void erase_from_chunks( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnReleasing );
    size_t eraseNum = chunk_inn::chunks.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
    esrc::erase_chunkKey_from_onReleasing(chunkKey_);
}



/* 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * 找到对应的 mapEnt, 将其指针返回出去
 */
std::pair<ChunkMemState, MemMapEnt*> getnc_memMapEntPtr( IntVec2 anyMPos_ ){

    chunkKey_t chunkKey = anyMPos_2_chunkKey( anyMPos_ );
    //---
    auto chunkMemState = get_chunkMemState(chunkKey);
    if( chunkMemState != ChunkMemState::Active ){
        return {chunkMemState, nullptr}; // let caller handle the exception
    } 
    //---
    IntVec2  lMPosOff = get_chunk_lMPosOff( anyMPos_ );
    return { chunkMemState, chunk_inn::chunks.at(chunkKey)->getnc_mapEntPtr(lMPosOff) };
}




std::pair<ChunkMemState, Chunk*> get_chunkPtr( chunkKey_t key_ ){

    auto state = get_chunkMemState(key_);
    return (state == ChunkMemState::Active) ?
                std::pair<ChunkMemState, Chunk*> { state, chunk_inn::chunks.at(key_).get() } :
                std::pair<ChunkMemState, Chunk*> { state, nullptr }; // let caller handle the exception
                
}

// 仅在 chunkRelease::release_one_chunk() 中被调用
Chunk &get_chunkRef_onReleasing( chunkKey_t key_ ){
        tprAssert( get_chunkMemState(key_) == ChunkMemState::OnReleasing );
    return *(chunk_inn::chunks.at(key_));
}


}//---------------------- namespace: esrc -------------------------//

