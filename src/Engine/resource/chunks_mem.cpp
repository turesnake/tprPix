/*
 * ========================= chunks_mem.cpp ==========================
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
#include "chunkKey.h"

#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                insert_new_chunk
 * -----------------------------------------------------------
 * param: _anyMPos --  目标chunk 区域中的任何一个 mapent.mpos
 * 初始化以下数据：
 *  - chunkKey
 *  - mcpos
 */
Chunk *insert_new_chunk( const IntVec2 &_anyMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    Chunk chunk {};
    chunk.set_by_anyMPos( _anyMPos );
    chunkKey_t key = chunk.get_key();
        assert( esrc::chunks.find(key) == esrc::chunks.end() );//- must not exist
    esrc::chunks.insert({ key, chunk }); //- copy
    //-----
    return (Chunk*)&(esrc::chunks.at(key));
}


/* ===========================================================
 *                  get_memMapEntPtr
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 chunk, 从 chunk.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 chunk 不存在，就要：加载它／创建它
 * -----------------
 * 这组函数存在缺陷：
 *   如果 mapent 所在的 chunk 并不存在，将直接出错。
 *   所以只能适用于少数场合
 */
MemMapEnt *get_memMapEntPtr( const MapCoord &_anyMCpos ){

    //-- 计算 目标 chunk 的 key --
    const IntVec2 &mposRef = _anyMCpos.get_mpos();
    chunkKey_t   chunkKey {};
    chunkKey = anyMPos_2_chunkKey( mposRef );
    //-- 拿着key，到 全局容器 esrc::chunks 中去找。--
        assert( esrc::chunks.find(chunkKey) != esrc::chunks.end() ); //- tmp
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff(mposRef);
    return esrc::chunks.at(chunkKey).get_mapEntPtr_by_lMPosOff( lMPosOff );
}

MemMapEnt *get_memMapEntPtr( const IntVec2 &_anyMPos ){

    //-- 计算 目标 chunk 的 key --
    const IntVec2 &mposRef = _anyMPos;
    chunkKey_t   chunkKey {};
    chunkKey = anyMPos_2_chunkKey( mposRef );
    //-- 拿着key，到 全局容器 esrc::chunks 中去找。--
        assert( esrc::chunks.find(chunkKey) != esrc::chunks.end() ); //- tmp
    //-- 获得 目标 mapEnt 在 chunk内部的 相对mpos
    IntVec2  lMPosOff = get_chunk_lMPosOff(mposRef);
    return esrc::chunks.at(chunkKey).get_mapEntPtr_by_lMPosOff( lMPosOff );
}


/* ===========================================================
 *              insert_new_chunkFieldSet    [可能被废弃]
 * -----------------------------------------------------------
 * param: _chunkKey -- 必须是 chunkKey.key
 * 已初始化 一阶数据 ...
 */
/*
ChunkFieldSet *insert_new_chunkFieldSet( chunkKey_t _chunkKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    ChunkFieldSet chunkFieldSet {};
    chunkFieldSet.set_by_chunkKey( _chunkKey );
        assert( esrc::chunkFieldSets.find(_chunkKey) == esrc::chunkFieldSets.end() );//- must not exist
    esrc::chunkFieldSets.insert({ _chunkKey, chunkFieldSet }); //- copy
    //-----
    return (ChunkFieldSet*)&(esrc::chunkFieldSets.at(_chunkKey));
}
*/


/* ===========================================================
 *              insert_new_chunkFieldSet
 * -----------------------------------------------------------
 * param: _anyMPos --  目标chunk 区域中的任何一个 mapent.mpos
 */
/*
ChunkFieldSet *insert_new_chunkFieldSet( const IntVec2 &_anyMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    ChunkFieldSet chunkFieldSet {};
    chunkFieldSet.set_by_anyMPos( _anyMPos );
    chunkKey_t key = chunkFieldSet.chunkKey;
        assert( esrc::chunkFieldSets.find(key) == esrc::chunkFieldSets.end() );//- must not exist
    esrc::chunkFieldSets.insert({ key, chunkFieldSet }); //- copy
    //-----
    return (ChunkFieldSet*)&(esrc::chunkFieldSets.at(key));
}
*/



}//---------------------- namespace: esrc -------------------------//

