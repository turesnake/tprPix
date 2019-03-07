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
 

IntVec2 chunkKey_2_mpos( chunkKey_t _key );
IntVec2 anyMPos_2_chunkMPos( const IntVec2 &_mpos );
IntVec2 get_chunk_lMPosOff( const IntVec2 &_anyMPos );
chunkKey_t anyMPos_2_chunkKey( const IntVec2 &_anyMPos );
size_t get_chunkIdx_in_section( const IntVec2 &_anyMPos );


namespace{//-------- namespace: --------------//

/* ===========================================================
 *             chunkMPos_2_key      [内部使用]
 * -----------------------------------------------------------
 * -- 传入 chunk左下角mpos，获得 chunk key（u64）
 */
inline chunkKey_t chunkMPos_2_key( const IntVec2 &_chunkMPos ){
    chunkKey_t key;
    int *ptr = (int*)&key;
    *ptr = _chunkMPos.x;
    ptr++;
    *ptr = _chunkMPos.y; 
    //--------
    return key;
}

}//------------- namespace: end --------------//


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
 * ------
 * param: _mpos -- 任意 mapent 的 mpos
 */
inline chunkKey_t anyMPos_2_chunkKey( const IntVec2 &_anyMPos ){
    //-- “地板除法，向低取节点值”, 再乘回 节点间距。
    //   获得 所在chunk 左下ent mpos
    IntVec2 chunkMPos = anyMPos_2_chunkMPos( _anyMPos );
    return chunkMPos_2_key( chunkMPos );
}


#endif 

