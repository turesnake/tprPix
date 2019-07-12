/*
 * ================== esrc_chunkMemState.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.10
 *                                        MODIFY --
 * ----------------------------------------------------------
 * only included by esrc_chunk.h
 */
#ifndef TPR_ESRC_CHUNK_MEM_STATE_H
#define TPR_ESRC_CHUNK_MEM_STATE_H

//-------------------- Engine --------------------//
#include "Chunk.h" 
#include "chunkKey.h"
#include "IntVec.h"
#include "ChunkMemState.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


//-- chunkKeys --
void insert_2_chunkKeys_onCreating( chunkKey_t chunkKey_ );
void move_chunkKey_from_onCreating_2_active( chunkKey_t chunkKey_ );
chunkKey_t pop_front_from_WaitForRelease_and_move_2_onReleasing();

ChunkMemState get_chunkMemState( chunkKey_t chunkKey_ );

void chunkMemState_debug( chunkKey_t key_, const std::string &str_ );


}//---------------------- namespace: esrc -------------------------//
#endif 

