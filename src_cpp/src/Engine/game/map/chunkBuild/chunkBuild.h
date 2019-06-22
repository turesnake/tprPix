/*
 * =================== chunkBuild.h =======================
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

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "fieldKey.h"
#include "chunkKey.h"


namespace chunkBuild {//------- namespace: chunkBuild ----------//
    

void build_9_chunks( const IntVec2 &_playerMPos );
void collect_chunks_need_to_be_build_in_update();


//-- 基于多线程的 新模块 --
chunkKey_t chunkBuild_3_receive_data_and_build_one_chunk();
void chunkBuild_4_wait_until_target_chunk_builded( chunkKey_t _chunkKey );


}//----------------- namespace: chunkBuild: end -------------------//
#endif

