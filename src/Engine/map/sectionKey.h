/*
 * ====================== sectionKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    section "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef TPR_SECTION_KEY_H
#define TPR_SECTION_KEY_H

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h" 
#include "MapCoord.h"

 
using sectionKey_t = uint64_t;

sectionKey_t sectionMPos_2_key_inn( IntVec2 sectionMPos_ )noexcept;  //- 不推荐外部代码使用
IntVec2 sectionKey_2_mpos( sectionKey_t key_ )noexcept;
IntVec2 anyMPos_2_sectionMPos( IntVec2 mpos_ )noexcept;
IntVec2 get_section_lMPosOff( IntVec2 anyMPos_ )noexcept;
sectionKey_t anyMPos_2_sectionKey( IntVec2 anyMPos_ )noexcept;
sectionKey_t sectionMPos_2_sectionKey( IntVec2 sectionMPos_ )noexcept;



/* ===========================================================
 *                 sectionMPos_2_key_inn
 * -----------------------------------------------------------
 * -- 传入 section左下角mpos，获得 section key（u64）
 * param: sectionMPos_ - 必须为 section左下角mpos。
 */
inline sectionKey_t sectionMPos_2_key_inn( IntVec2 sectionMPos_ )noexcept{
    sectionKey_t key {};
    int *ptr = (int*)&key;
    *ptr = sectionMPos_.x;
    ptr++;
    *ptr = sectionMPos_.y; 
    //--------
    return key;
}


/* ===========================================================
 *                sectionKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 section 的 mpos
 */
inline IntVec2 sectionKey_2_mpos( sectionKey_t key_ )noexcept{
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
 *             anyMPos_2_sectionMPos
 * -----------------------------------------------------------
 * -- 传入 任意 mapent 的 mpos，获得其 所在 section 的 mpos（section左下角）
 */
inline IntVec2 anyMPos_2_sectionMPos( IntVec2 anyMPos_ )noexcept{
    return ( floorDiv(anyMPos_, ENTS_PER_SECTION_D) * ENTS_PER_SECTION<> );
}


/* ===========================================================
 *                get_section_lMPosOff
 * -----------------------------------------------------------
 * -- 获得 目标mapent.mpos 在其 section 中的 相对mpos偏移
 */
inline IntVec2 get_section_lMPosOff( IntVec2 anyMPos_ )noexcept{
    return ( anyMPos_ - anyMPos_2_sectionMPos(anyMPos_) );
}


/* ===========================================================
 *               anyMPos_2_sectionKey
 * -----------------------------------------------------------
 * -- 当需要通过 mpos 计算出它的 key，又不需要 正式制作一个 SectionKey实例时，
 *    推荐使用本函数。
 * -- 这个函数会使得调用者代码 隐藏一些bug。
 *    在明确自己传入的参数就是 sectionMPos 时，推荐使用 sectionMPos_2_sectionKey()
 * param: anyMPos_ -- 任意 mapent 的 mpos
 */
inline sectionKey_t anyMPos_2_sectionKey( IntVec2 anyMPos_ )noexcept{
    IntVec2 sectionMPos = anyMPos_2_sectionMPos( anyMPos_ );
    return sectionMPos_2_key_inn( sectionMPos );
}


/* ===========================================================
 *               sectionMPos_2_sectionKey
 * -----------------------------------------------------------
 * -- 当使用者 确定自己传入的参数就是 sectionMPos, 使用此函数
 *    如果参数不为 sectionMPos，直接报错。
 */
inline sectionKey_t sectionMPos_2_sectionKey( IntVec2 sectionMPos_ )noexcept{
        tprAssert( anyMPos_2_sectionMPos(sectionMPos_) == sectionMPos_ ); //- tmp
    return sectionMPos_2_key_inn( sectionMPos_ );
}


#endif 

