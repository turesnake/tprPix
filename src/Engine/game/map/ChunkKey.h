/*
 * ====================== chunkKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Chunk "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef _TPR_CHUNK_KEY_H_
#define _TPR_CHUNK_KEY_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h" 
#include "MapCoord.h"


using chunkKey_t = u64_t;
 
chunkKey_t chunkMPos_2_key_inn( const IntVec2 &_chunkMPos ); //- 不推荐外部代码使用
IntVec2 chunkKey_2_mpos( chunkKey_t _key );
IntVec2 anyMPos_2_chunkMPos( const IntVec2 &_mpos );
IntVec2 get_chunk_lMPosOff( const IntVec2 &_anyMPos );
chunkKey_t anyMPos_2_chunkKey( const IntVec2 &_anyMPos );
chunkKey_t chunkMPos_2_chunkKey( const IntVec2 &_chunkMPos );
size_t get_chunkIdx_in_section( const IntVec2 &_anyMPos );




/* ===========================================================
 *             chunkMPos_2_key_inn      [内部使用]
 * -----------------------------------------------------------
 * -- 传入 chunk左下角mpos，获得 chunk key（u64）
 */
inline chunkKey_t chunkMPos_2_key_inn( const IntVec2 &_chunkMPos ){
    chunkKey_t key;
    int *ptr = (int*)&key;
    *ptr = _chunkMPos.x;
    ptr++;
    *ptr = _chunkMPos.y; 
    //--------
    return key;
}



/* ===========================================================
 *                chunkKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 chunk 的 mpos
 */
inline IntVec2 chunkKey_2_mpos( chunkKey_t _key ){
    IntVec2  mpos {};
    int *ptr = (int*)&_key;
    //---
    mpos.x = *ptr;
    ptr++;
    mpos.y = *ptr;
    //---
    return mpos;
} 


/* ===========================================================
 *             anyMPos_2_chunkMPos
 * -----------------------------------------------------------
 * -- 传入 任意 mapent 的 mpos，获得其 所在 chunk 的 mpos（chunk左下角）
 */
inline IntVec2 anyMPos_2_chunkMPos( const IntVec2 &_anyMPos ){
    return ( floorDiv(_anyMPos,ENTS_PER_CHUNK) * ENTS_PER_CHUNK );
}


/* ===========================================================
 *             is_a_chunkMPos
 * -----------------------------------------------------------
 * -- 检测 目标参数， 是否为 chunk mpos （多用于 assert ）
 */
inline bool is_a_chunkMPos( const IntVec2 &_anyMPos ){
    return ( anyMPos_2_chunkMPos(_anyMPos) == _anyMPos ); //- 不需要考虑性能
}


/* ===========================================================
 *                get_chunk_lMPosOff
 * -----------------------------------------------------------
 * -- 获得 目标mapent.mpos 在其 chunk 中的 相对mpos偏移
 */
inline IntVec2 get_chunk_lMPosOff( const IntVec2 &_anyMPos ){
    return ( _anyMPos - anyMPos_2_chunkMPos(_anyMPos) );
}


/* ===========================================================
 *             anyMPos_2_chunkKey
 * -----------------------------------------------------------
 * -- 当需要通过 mpos 计算出它的 key，又不需要 正式制作一个 ChunkKey实例时，
 *    推荐使用本函数。
 * -- 这个函数会使得调用者代码 隐藏一些bug。
 *    在明确自己传入的参数就是 chunkMPos 时，推荐使用 chunkMPos_2_chunkKey()
 * param: _anyMPos -- 任意 mapent 的 mpos
 */
inline chunkKey_t anyMPos_2_chunkKey( const IntVec2 &_anyMPos ){
    IntVec2 chunkMPos = anyMPos_2_chunkMPos( _anyMPos );
    return chunkMPos_2_key_inn( chunkMPos );
}

/* ===========================================================
 *             chunkMPos_2_chunkKey
 * -----------------------------------------------------------
 * -- 当使用者 确定自己传入的参数就是 chunkMPos, 使用此函数
 *    如果参数不为 chunkMPos，直接报错。
 */
inline chunkKey_t chunkMPos_2_chunkKey( const IntVec2 &_chunkMPos ){
        assert( anyMPos_2_chunkMPos(_chunkMPos) == _chunkMPos ); //- tmp
    return chunkMPos_2_key_inn( _chunkMPos );
}


#endif 

