/*
 * =================== chunkRelease.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHUNK_RELEASE_H
#define TPR_CHUNK_RELEASE_H

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "fieldKey.h"
#include "chunkKey.h"

namespace chunkRelease {//------- namespace: chunkRelease ----------//

void release_chunk( chunkKey_t chunkKey_ );


}//----------------- namespace: chunkRelease: end -------------------//
#endif 

