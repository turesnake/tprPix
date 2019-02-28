/*
 * ========================= chunk_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk 内存态
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "config.h"
#include "ChunkKey.h"

#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                insert_new_chunk
 * -----------------------------------------------------------
 * -- 不是一个 完善的 chunk 生成器。仅能用于 bypass 阶段
 * -- 注意： 返回的 chunk 是全空的 ！！！
 * ------
 * param: _chunkMCPos -- “推荐”使用 chunk左下角 mcpos
 */
Chunk *insert_new_chunk( const MapCoord &_chunkMCPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    Chunk chunk {};
    chunk.set_by_mapEnt_mpos( _chunkMCPos.get_mpos() );
    u64_t key = chunk.get_key();
    esrc::chunks.insert({ key, chunk }); //- copy

        //cout << "chunks.size() = " << esrc::chunks.size() << endl;
    //-----
    return (Chunk*)&(esrc::chunks.at(key));
}


/* ===========================================================
 *                  get_memMapEnt
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 chunk 不存在，就要：加载它／创建它
 * ------
 * param: _mcpos -- 任意mapent 的 mcpos
 */
MemMapEnt *get_memMapEnt( const MapCoord &_mcpos ){

    //-- 计算 目标 chunk 的 key --
    const IntVec2 &mposRef = _mcpos.get_mpos();
    ChunkKey  chunkKey {};
    chunkKey.init_by_mapEnt_mpos( mposRef );

    //-- 拿着key，到 全局容器 esrc::chunks 中去找。--
        assert( esrc::chunks.find(chunkKey.get_key()) != esrc::chunks.end() ); //- tmp
    Chunk &chunkRef = esrc::chunks.at(chunkKey.get_key());

    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff(mposRef);
    return chunkRef.get_memMapEnt_by_lMPosOff( lMPosOff );
}


/* ===========================================================
 *              insert_new_chunkFieldSet
 * -----------------------------------------------------------
 * param: _chunkMCPos -- 必须是 chunk左下角 mcpos
 */
ChunkFieldSet *insert_new_chunkFieldSet( u64_t _chunkKeyVal ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapCoord chunkMCPos = chunkKey_2_mcpos(_chunkKeyVal);
    ChunkFieldSet fieldSet {};
    fieldSet.init( chunkMCPos.get_mpos() );
    esrc::chunkFieldSets.insert({ _chunkKeyVal, fieldSet }); //- copy
    //-----
    return (ChunkFieldSet*)&(esrc::chunkFieldSets.at(_chunkKeyVal));
}



}//---------------------- namespace: esrc -------------------------//

