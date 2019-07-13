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

void collect_chunks_need_to_be_release_in_update();
void release_one_chunk();

}//----------------- namespace: chunkRelease: end -------------------//
#endif 

