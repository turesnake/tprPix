/*
 * ====================== fieldKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    MapField "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef _TPR_FIELD_KEY_H_
#define _TPR_FIELD_KEY_H_


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


using fieldKey_t = u64_t;

fieldKey_t fieldMPos_2_key( const IntVec2 &_fieldMPos ); //- 不推荐外部代码使用
IntVec2 fieldKey_2_mpos( fieldKey_t _key );
IntVec2 anyMPos_2_fieldMPos( const IntVec2 &_anyMPos );
fieldKey_t anyMPos_2_fieldKey( const IntVec2 &_anyMPos );



/* ===========================================================
 *                fieldMPos_2_key
 * -----------------------------------------------------------
 * -- 传入 field 左下角mpos，获得 field key val
 *   此函数并不安全（没有检测 参数是否为 fieldMpos）所以只能在模块内部使用
 * param: _fieldMPos - 必须为 field 左下角mpos。
 */
inline fieldKey_t fieldMPos_2_key( const IntVec2 &_fieldMPos ){
    fieldKey_t key;
    int *ptr = (int*)&key;
    *ptr = _fieldMPos.x;
    ptr++;
    *ptr = _fieldMPos.y; 
    //--------
    return key;
}


/* ===========================================================
 *                fieldKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 field 的 mpos
 */
inline IntVec2 fieldKey_2_mpos( fieldKey_t _key ){
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
 *             anyMPos_2_fieldMPos
 * -----------------------------------------------------------
 * -- 传入 任意 mapent 的 mpos，获得其 所在 field 的 mpos（chunk左下角）
 */
inline IntVec2 anyMPos_2_fieldMPos( const IntVec2 &_anyMPos ){
    return ( floorDiv(_anyMPos,ENTS_PER_FIELD) * ENTS_PER_FIELD );
}


/* ===========================================================
 *               anyMPos_2_fieldKey
 * -----------------------------------------------------------
 * param: _mpos -- 任意 mapent 的 mpos
 */
inline fieldKey_t anyMPos_2_fieldKey( const IntVec2 &_anyMPos ){
    IntVec2 fieldMPos = anyMPos_2_fieldMPos( _anyMPos );
    return fieldMPos_2_key( fieldMPos );
}



#endif 

