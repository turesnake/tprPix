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
#include <optional>

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "fieldKey.h"
#include "chunkKey.h"


namespace chunkCreate {//------- namespace: chunkCreate ----------//
    

void create_9_chunks( IntVec2 playerMPos_ );
void collect_chunks_need_to_be_create_in_update();


void create_chunks_from_waitingQue();


//-- 基于多线程的 新模块 --
std::optional<chunkKey_t> chunkCreate_3_receive_data_and_create_one_chunk();


void collect_ecoObjs_need_to_be_create();


}//----------------- namespace: chunkCreate: end -------------------//
#endif

