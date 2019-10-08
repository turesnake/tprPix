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
#include "chunkCreate.h"
#include "esrc_renderPool.h"
#include "esrc_state.h"

#include "tprDebug.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace chunk_inn {//------------ namespace: chunk_inn --------------//
    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, std::unique_ptr<Chunk>> chunks {};

    std::unique_ptr<ChunkCreateReleaseZone> chunkCreateReleaseZoneUPtr;

}//---------------- namespace: chunk_inn end --------------//

extern void chunkStates_debug();
extern void erase_chunkKey_from_onReleasing( chunkKey_t chunkKey_ );
extern const std::unordered_set<chunkKey_t> &get_chunkKeys_active();


void init_chunks(){
    chunk_inn::chunks.reserve(1000);
    chunk_inn::chunkCreateReleaseZoneUPtr = std::make_unique<ChunkCreateReleaseZone>( 3, 7 );
                                                                // 也许应该改为 3,9
    //...
    esrc::insertState("chunk");
}


void chunks_debug(){
    cout << "\nchunks.size() = " << chunk_inn::chunks.size() 
        << endl;
    chunkStates_debug();
}


void init_chunkCreateReleaseZone( IntVec2 playerMPos_ ){
    chunk_inn::chunkCreateReleaseZoneUPtr->init( playerMPos_ );
}

ChunkCreateReleaseZone &get_chunkCreateReleaseZoneRef(){
    return *(chunk_inn::chunkCreateReleaseZoneUPtr);
}

//- only used for esrc_chunkMemState -
bool find_from_chunks( chunkKey_t chunkKey_ ){
    return (chunk_inn::chunks.find(chunkKey_)!=chunk_inn::chunks.end());
}



/* ===========================================================
 *             insert_and_init_new_chunk
 * -----------------------------------------------------------
 * 创建 chunk实例，放入 全局容器，且初始化它
 * 仅被 check_and_build_sections.cpp -> create_one_chunk() 调用
 */
Chunk &insert_and_init_new_chunk( chunkKey_t chunkKey_ ){
    auto outPair = chunk_inn::chunks.insert({ chunkKey_, std::make_unique<Chunk>( chunkKey_ ) });
    tprAssert( outPair.second );
    Chunk &chunkRef = *(outPair.first->second);

    esrc::move_chunkKey_from_onCreating_2_active(chunkKey_);  
                            // Now, the chunkState is Active !!! 
    return chunkRef;
}


/* ===========================================================
 *             erase_from_chunks
 * -----------------------------------------------------------
 */
void erase_from_chunks( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnReleasing );
    size_t eraseNum = chunk_inn::chunks.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
    esrc::erase_chunkKey_from_onReleasing(chunkKey_);
}



/* ===========================================================
 *               getnc_memMapEntRef
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 *    当目标 chunkMemState 不为 Active，直接报错
 */
MemMapEnt &getnc_memMapEntRef( IntVec2 anyMPos_ ){

    chunkKey_t chunkKey = anyMPos_2_chunkKey( anyMPos_ );
    IntVec2  lMPosOff = get_chunk_lMPosOff( anyMPos_ );

        //-- Frequent Bugs --
        if( get_chunkMemState(chunkKey) != ChunkMemState::Active ){
            std::string info = "\nesrc::getnc_memMapEntRef(): ";
            esrc::chunkMemState_debug( chunkKey, info );
        }

    tprAssert( get_chunkMemState(chunkKey) == ChunkMemState::Active );
    return chunk_inn::chunks.at(chunkKey)->getnc_mapEntRef( lMPosOff );
}



/* ===========================================================
 *                 get_chunkRef
 * -----------------------------------------------------------
 */
Chunk &get_chunkRef( chunkKey_t key_ ){


                if( get_chunkMemState(key_) != ChunkMemState::Active ){
                    IntVec2 chunkMPos = chunkKey_2_mpos(key_);
                    cout << "\nERROR: get_chunkRef; chunkMPos: " << chunkMPos.x 
                        << ", " << chunkMPos.y 
                        << endl;
                    chunkMemState_debug( key_, "" );
                }
        
        tprAssert( get_chunkMemState(key_) == ChunkMemState::Active );
    return *(chunk_inn::chunks.at(key_));
}

/* ===========================================================
 *              get_chunkRef_onReleasing
 * -----------------------------------------------------------
 * 仅在 chunkRelease::release_one_chunk() 中被调用
 */
Chunk &get_chunkRef_onReleasing( chunkKey_t key_ ){
        tprAssert( get_chunkMemState(key_) == ChunkMemState::OnReleasing );
    return *(chunk_inn::chunks.at(key_));
}


}//---------------------- namespace: esrc -------------------------//

