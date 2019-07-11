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
    //- chunkMemState 已在 chunkBuild_3_receive_data_and_build_one_chunk() 中被改为: Active
    return *(chunk_inn::chunks.at(chunkKey_).get());
}


/* ===========================================================
 *                  get_memMapEntRef
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 chunk 不存在，就要：加载它／创建它
 * -----------------
 * 这组函数存在缺陷：
 *   如果 mapent 所在的 chunk 并不存在，将直接出错。
 *   尤其是 collision 模块中
 *   ----
 *   目前的做法是，当发现目标 chunk 不存在时，调用一个 特殊的函数，阻塞主线程，直到目标chunk制作好
 *   这个方法是防止程序崩溃的最后办法，临时的
 *   未来希望更好的办法...
 */
MemMapEnt &get_memMapEntRef( const MapCoord &anyMCpos_ ){

    //-- 计算 目标 chunk 的 key --
    const IntVec2 &mposRef = anyMCpos_.get_mpos();
    chunkKey_t     chunkKey = anyMPos_2_chunkKey( mposRef );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( mposRef );

                auto chunkState = get_chunkMemState(chunkKey);
                switch (chunkState){
                case ChunkMemState::NotExist:
                    cout << "get_memMapEntRef(): target chunk is not exist..." << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                    tprAssert( get_chunkMemState(chunkKey) == ChunkMemState::Active ); //- 再次检测
                    break;

                case ChunkMemState::OnCreating:
                    cout << "get_memMapEntRef()[anyMCpos]: target chunk is on creating..." << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                    tprAssert( get_chunkMemState(chunkKey) == ChunkMemState::Active ); //- 再次检测
                    break;

                case ChunkMemState::Active:
                    // do nothing
                    break;

                case ChunkMemState::WaitForRelease:
                    cout << "get_memMapEntRef(): target chunk is wait for release..." << endl;
                    tprAssert(0); // tmp
                    break;

                case ChunkMemState::OnReleasing:
                    cout << "get_memMapEntRef(): target chunk is on releasing..." << endl;
                    tprAssert(0); // tmp
                    break;
                
                default:
                    tprAssert(0);
                    break;
                }
    return chunk_inn::chunks.at(chunkKey)->getnc_mapEntRef_by_lMPosOff( lMPosOff );
}

MemMapEnt &get_memMapEntRef( const IntVec2 &anyMPos_ ){

    //-- 计算 目标 chunk 的 key --
    chunkKey_t    chunkKey = anyMPos_2_chunkKey( anyMPos_ );
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff( anyMPos_ );

                auto chunkState = get_chunkMemState(chunkKey);
                switch (chunkState){
                case ChunkMemState::NotExist:
                    cout << "get_memMapEntRef(): target chunk is not exist..." << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                    tprAssert( get_chunkMemState(chunkKey) == ChunkMemState::Active ); //- 再次检测
                    break;

                case ChunkMemState::OnCreating:
                    cout << "get_memMapEntRef()[anyMPos]: target chunk is on creating..." << endl;
                    chunkBuild::chunkBuild_4_wait_until_target_chunk_builded( chunkKey );
                    tprAssert( get_chunkMemState(chunkKey) == ChunkMemState::Active ); //- 再次检测
                    //tprAssert(0); // tmp
                    break;

                case ChunkMemState::Active:
                    // do nothing
                    break;

                case ChunkMemState::WaitForRelease:
                    cout << "get_memMapEntRef(): target chunk is wait for release..." << endl;
                    tprAssert(0); // tmp
                    break;

                case ChunkMemState::OnReleasing:
                    cout << "get_memMapEntRef(): target chunk is on releasing..." << endl;
                    tprAssert(0); // tmp
                    break;
                
                default:
                    tprAssert(0);
                    break;
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
 */
void render_chunks(){
    for( auto& p : chunk_inn::chunks ){
            p.second->refresh_translate_auto(); //-- MUST !!!
            insert_2_renderPool_meshs( p.second->get_mesh().get_render_z(),
                                        const_cast<Mesh*>(&p.second->get_mesh()) );
    }
}


}//---------------------- namespace: esrc -------------------------//

