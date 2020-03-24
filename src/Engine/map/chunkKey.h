/*
 * ====================== chunkKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Chunk "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef TPR_CHUNK_KEY_H
#define TPR_CHUNK_KEY_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h" 
#include "MapCoord.h"


using chunkKey_t = uint64_t;
 
chunkKey_t chunkMPos_2_key_inn( IntVec2 chunkMPos_ )noexcept; //- 不推荐外部代码使用
IntVec2 chunkKey_2_mpos( chunkKey_t key_ )noexcept;
IntVec2 anyMPos_2_chunkMPos( IntVec2 mpos_ )noexcept;
IntVec2 get_chunk_lMPosOff( IntVec2 anyMPos_ )noexcept;
chunkKey_t anyMPos_2_chunkKey( IntVec2 anyMPos_ )noexcept;
chunkKey_t chunkMPos_2_chunkKey( IntVec2 chunkMPos_ )noexcept;
size_t get_chunkIdx_in_section( IntVec2 anyMPos_ )noexcept;
IntVec2 chunkMPos_2_chunkCPos( IntVec2 chunkMPos_ )noexcept;


/* ===========================================================
 *             chunkMPos_2_key_inn      [内部使用]
 * -----------------------------------------------------------
 * -- 传入 chunk左下角mpos，获得 chunk key（u64）
 */
inline chunkKey_t chunkMPos_2_key_inn( IntVec2 chunkMPos_ )noexcept{
    chunkKey_t key {};
    int *ptr = (int*)(&key); //- 此处不能使用 static_casts
    *ptr = chunkMPos_.x;
    ptr++;
    *ptr = chunkMPos_.y; 
    //--------
    return key;
}



/* ===========================================================
 *                chunkKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 chunk 的 mpos
 */
inline IntVec2 chunkKey_2_mpos( chunkKey_t key_ )noexcept{
    IntVec2  mpos {};
    int *ptr = (int*)&key_;
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
inline IntVec2 anyMPos_2_chunkMPos( IntVec2 anyMPos_ )noexcept{
    return ( floorDiv(anyMPos_, ENTS_PER_CHUNK_D) * ENTS_PER_CHUNK<> );
}



/* ===========================================================
 *             is_a_chunkMPos
 * -----------------------------------------------------------
 * -- 检测 目标参数， 是否为 chunk mpos （多用于 tprAssert ）
 */
inline bool is_a_chunkMPos( IntVec2 anyMPos_ )noexcept{
    return ( anyMPos_2_chunkMPos(anyMPos_) == anyMPos_ ); //- 不需要考虑性能
}


/* ===========================================================
 *                get_chunk_lMPosOff
 * -----------------------------------------------------------
 * -- 获得 目标mapent.mpos 在其 chunk 中的 相对mpos偏移
 */
inline IntVec2 get_chunk_lMPosOff( IntVec2 anyMPos_ )noexcept{
    return ( anyMPos_ - anyMPos_2_chunkMPos(anyMPos_) );
}


/* ===========================================================
 *             anyMPos_2_chunkKey
 * -----------------------------------------------------------
 * -- 当需要通过 mpos 计算出它的 key，又不需要 正式制作一个 ChunkKey实例时，
 *    推荐使用本函数。
 * -- 这个函数会使得调用者代码 隐藏一些bug。
 *    在明确自己传入的参数就是 chunkMPos 时，推荐使用 chunkMPos_2_chunkKey()
 * param: anyMPos_ -- 任意 mapent 的 mpos
 */
inline chunkKey_t anyMPos_2_chunkKey( IntVec2 anyMPos_ )noexcept{
    IntVec2 chunkMPos = anyMPos_2_chunkMPos( anyMPos_ );
    return chunkMPos_2_key_inn( chunkMPos );
}


inline chunkKey_t anyDPos_2_chunkKey( const glm::dvec2 &anyDPos_ )noexcept{
    IntVec2 chunkMPos = anyMPos_2_chunkMPos( dpos_2_mpos(anyDPos_) ); //-- 未来做优化 
    return chunkMPos_2_key_inn( chunkMPos );
}


/* ===========================================================
 *             chunkMPos_2_chunkKey
 * -----------------------------------------------------------
 * -- 当使用者 确定自己传入的参数就是 chunkMPos, 使用此函数
 *    如果参数不为 chunkMPos，直接报错。
 */
inline chunkKey_t chunkMPos_2_chunkKey( IntVec2 chunkMPos_ )noexcept{
        tprAssert( anyMPos_2_chunkMPos(chunkMPos_) == chunkMPos_ ); //- tmp
    return chunkMPos_2_key_inn( chunkMPos_ );
}

/* ===========================================================
 *             chunkMPos_2_chunkCPos
 * -----------------------------------------------------------
 * -- 当使用者 确定自己传入的参数就是 chunkMPos, 使用此函数
 *    如果参数不为 chunkMPos，直接报错。
 */
inline IntVec2 chunkMPos_2_chunkCPos( IntVec2 chunkMPos_ )noexcept{
        tprAssert( anyMPos_2_chunkMPos(chunkMPos_) == chunkMPos_ ); //- tmp
    return floorDiv( chunkMPos_, ENTS_PER_CHUNK_D );
}


#endif 

