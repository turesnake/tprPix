/*
 * ====================== fieldKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    MapField "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef TPR_FIELD_KEY_H
#define TPR_FIELD_KEY_H

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h" 
#include "MapCoord.h"


using fieldKey_t = uint64_t;

fieldKey_t fieldMPos_2_key_inn( IntVec2 fieldMPos_ )noexcept; //- 不推荐外部代码使用
IntVec2 fieldKey_2_mpos( fieldKey_t key_ )noexcept;
IntVec2 anyMPos_2_fieldMPos( IntVec2 anyMPos_ )noexcept;
fieldKey_t anyMPos_2_fieldKey( IntVec2 anyMPos_ )noexcept;
fieldKey_t anyDPos_2_fieldKey( const glm::dvec2 &anyDPos_ )noexcept;
fieldKey_t fieldMPos_2_fieldKey( IntVec2 fieldMPos_ )noexcept;


/* ===========================================================
 *                fieldMPos_2_key_inn
 * -----------------------------------------------------------
 * -- 传入 field 左下角mpos，获得 field key val
 *   此函数并不安全（没有检测 参数是否为 fieldMpos）所以只能在模块内部使用
 * param: fieldMPos_ - 必须为 field 左下角mpos。
 */
inline fieldKey_t fieldMPos_2_key_inn( IntVec2 fieldMPos_ )noexcept{
    fieldKey_t key {};
    int *ptr = (int*)&key;
    *ptr = fieldMPos_.x;
    ptr++;
    *ptr = fieldMPos_.y; 
    //--------
    return key;
}


/* ===========================================================
 *                fieldKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 field 的 mpos
 */
inline IntVec2 fieldKey_2_mpos( fieldKey_t key_ )noexcept{
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
 *             anyMPos_2_fieldMPos
 * -----------------------------------------------------------
 * -- 传入 任意 mapent 的 mpos，获得其 所在 field 的 mpos（chunk左下角）
 */
inline IntVec2 anyMPos_2_fieldMPos( IntVec2 anyMPos_ )noexcept{
    return ( floorDiv(anyMPos_, ENTS_PER_FIELD_D) * ENTS_PER_FIELD<> );
}


/* ===========================================================
 *               anyMPos_2_fieldKey
 * -----------------------------------------------------------
 * -- 这个函数会使得调用者代码 隐藏一些bug。
 *    在明确自己传入的参数就是 fieldMPos 时，推荐使用 fieldMPos_2_sectionKey()
 * param: anyMPos_ -- 任意 mapent 的 mpos
 * 
 *  在目前版本中，mapent 通过此函数，简单地被分配到各个 field...
 *  这个办法并不完美，但迫不得已...
 * 
 */
inline fieldKey_t anyMPos_2_fieldKey( IntVec2 anyMPos_ )noexcept{
    IntVec2 fieldMPos = anyMPos_2_fieldMPos( anyMPos_ );
    return fieldMPos_2_key_inn( fieldMPos );
}


inline fieldKey_t anyDPos_2_fieldKey( const glm::dvec2 &anyDPos_ )noexcept{
    IntVec2 fieldMPos = anyMPos_2_fieldMPos( dpos_2_mpos(anyDPos_) ); //-- 未来做优化 
    return fieldMPos_2_key_inn( fieldMPos );
}


/* ===========================================================
 *               fieldMPos_2_fieldKey
 * -----------------------------------------------------------
 * -- 当使用者 确定自己传入的参数就是 fieldMPos, 使用此函数
 *    如果参数不为 fieldMPos，直接报错。
 */
inline fieldKey_t fieldMPos_2_fieldKey( IntVec2 fieldMPos_ )noexcept{
        tprAssert( anyMPos_2_fieldMPos(fieldMPos_) == fieldMPos_ ); //- tmp
    return fieldMPos_2_key_inn( fieldMPos_ );
}



#endif 

