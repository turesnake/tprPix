/*
 * ====================== sectionKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    section "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef _TPR_SECTION_KEY_H_
#define _TPR_SECTION_KEY_H_

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

 
using sectionKey_t = u64_t;


IntVec2 sectionKey_2_mpos( sectionKey_t _key );
IntVec2 anyMPos_2_sectionMPos( const IntVec2 &_mpos );
IntVec2 get_section_lMPosOff( const IntVec2 &_anyMPos );
sectionKey_t anyMPos_2_sectionKey( const IntVec2 &_anyMPos );


namespace{//-------- namespace: --------------//

/* ===========================================================
 *                 sectionMPos_2_key
 * -----------------------------------------------------------
 * -- 传入 section左下角mpos，获得 section key（u64）
 * param: _sectionMPos - 必须为 section左下角mpos。
 */
inline sectionKey_t sectionMPos_2_key( const IntVec2 &_sectionMPos ){
    sectionKey_t key;
    int *ptr = (int*)&key;
    *ptr = _sectionMPos.x;
    ptr++;
    *ptr = _sectionMPos.y; 
    //--------
    return key;
}

}//------------- namespace: end --------------//


/* ===========================================================
 *                sectionKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 section 的 mpos
 */
inline IntVec2 sectionKey_2_mpos( sectionKey_t _key ){
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
 *             anyMPos_2_sectionMPos
 * -----------------------------------------------------------
 * -- 传入 任意 mapent 的 mpos，获得其 所在 section 的 mpos（section左下角）
 */
inline IntVec2 anyMPos_2_sectionMPos( const IntVec2 &_anyMPos ){
    return ( floorDiv(_anyMPos, ENTS_PER_SECTION) * ENTS_PER_SECTION );
}


/* ===========================================================
 *                get_section_lMPosOff
 * -----------------------------------------------------------
 * -- 获得 目标mapent.mpos 在其 section 中的 相对mpos偏移
 */
inline IntVec2 get_section_lMPosOff( const IntVec2 &_anyMPos ){
    return ( _anyMPos - anyMPos_2_sectionMPos(_anyMPos) );
}


/* ===========================================================
 *               anyMPos_2_sectionKey
 * -----------------------------------------------------------
 * -- 当需要通过 mpos 计算出它的 key，又不需要 正式制作一个 SectionKey实例时，
 *    推荐使用本函数。
 * ------
 * param: _mpos -- 任意 mapent 的 mpos
 */
inline sectionKey_t anyMPos_2_sectionKey( const IntVec2 &_anyMPos ){
    //-- “地板除法，向低取节点值”, 再乘回 节点间距。
    //   获得 所在chunk 左下ent mpos
    IntVec2 sectionMPos = anyMPos_2_sectionMPos( _anyMPos );
    return sectionMPos_2_key( sectionMPos );
}


#endif 

