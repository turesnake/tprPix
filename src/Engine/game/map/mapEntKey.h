/*
 * ====================== mapEntKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    mapEnt "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef TPR_MAPENT_KEY_H
#define TPR_MAPENT_KEY_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h" 
#include "MapCoord.h"


using mapEntKey_t = u64_t;


inline mapEntKey_t mpos_2_key( IntVec2 mpos_ )noexcept{
    mapEntKey_t key {};
    int *ptr = (int*)(&key); //- can't use static_cast<>
    *ptr = mpos_.x;
    ptr++;
    *ptr = mpos_.y; 
    //--------
    return key;
}



/* ===========================================================
 *                chunkKey_2_mpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 chunk 的 mpos
 */
inline IntVec2 mapEntKey_2_mpos( mapEntKey_t key_ )noexcept{
    IntVec2  mpos {};
    int *ptr = (int*)&key_; //- can't use static_cast<>
    //---
    mpos.x = *ptr;
    ptr++;
    mpos.y = *ptr;
    //---
    return mpos;
} 


#endif 

