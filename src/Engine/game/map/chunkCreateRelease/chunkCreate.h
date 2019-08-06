/*
 * =================== chunkCreate.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk build api
 * ----------------------------
 */
#ifndef TPR_CHUNK_BUILD_H
#define TPR_CHUNK_BUILD_H

//------------------- CPP --------------------//
#include <vector>
#include <utility>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "fieldKey.h"
#include "chunkKey.h"


namespace chunkCreate {//------- namespace: chunkCreate ----------//
    

void build_9_chunks( const IntVec2 &playerMPos_ );
void collect_chunks_need_to_be_build_in_update();


//-- 基于多线程的 新模块 --
std::pair<bool,chunkKey_t> chunkBuild_3_receive_data_and_build_one_chunk();


}//----------------- namespace: chunkCreate: end -------------------//
#endif

