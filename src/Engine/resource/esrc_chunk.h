/*
 * ========================= esrc_chunk.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_CHUNK_H
#define TPR_ESRC_CHUNK_H

//-------------------- CPP --------------------//
#include <string>
#include <utility>

//-------------------- Engine --------------------//
#include "Chunk.h" 
#include "chunkKey.h"
#include "IntVec.h"
#include "ChunkCreateReleaseZone.h"
#include "ChunkMemState.h"
#include "esrc_chunkMemState.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void init_chunks();
void init_chunkCreateReleaseZone( IntVec2 playerMPos_ );

ChunkCreateReleaseZone &get_chunkCreateReleaseZoneRef();

void chunks_debug();

Chunk &insert_and_init_new_chunk( chunkKey_t chunkKey_ );
void erase_from_chunks( chunkKey_t chunkKey_ );

std::pair<ChunkMemState, MemMapEnt*> getnc_memMapEntPtr( IntVec2 anyMPos_ );


std::pair<ChunkMemState, Chunk*> get_chunkPtr( chunkKey_t key_ );
Chunk &get_chunkRef_onReleasing( chunkKey_t key_ );


}//---------------------- namespace: esrc -------------------------//
#endif

