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

//-------------------- CPP --------------------//
#include <utility>
#include <optional>

//-------------------- Engine --------------------//
#include "Chunk.h" 
#include "chunkKey.h"
#include "IntVec.h"
#include "ChunkMemState.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

void init_chunkMemStates()noexcept;

void chunkMemState_debug( chunkKey_t key_, const std::string &str_ ); // debug tmp

//-- chunkKeys --
void insert_chunkKey_2_waitForCreate( chunkKey_t chunkKey_ )noexcept;
void insert_chunkKey_2_onCreating( chunkKey_t chunkKey_ )noexcept;
const std::unordered_set<chunkKey_t> &get_chunkKeys_waitForCreate()noexcept;
void move_chunkKey_from_waitForCreate_2_onCreating( chunkKey_t chunkKey_ )noexcept;
void move_chunkKey_from_onCreating_2_active( chunkKey_t chunkKey_ )noexcept;
void move_chunkKey_from_active_2_waitForRelease( chunkKey_t chunkKey_ )noexcept;
std::optional<chunkKey_t> pop_front_from_WaitForRelease_and_move_2_onReleasing()noexcept;

ChunkMemState get_chunkMemState( chunkKey_t chunkKey_ )noexcept;


}//---------------------- namespace: esrc -------------------------//
#endif 

