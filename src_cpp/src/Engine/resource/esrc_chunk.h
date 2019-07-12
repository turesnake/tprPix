/*
 * ========================= esrc_chunk.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_CHUNK_H
#define TPR_ESRC_CHUNK_H

//-------------------- Engine --------------------//
#include "Chunk.h" 
#include "chunkKey.h"
#include "IntVec.h"
#include "ChunkCreateReleaseZone.h"
#include "ChunkMemState.h"
#include "esrc_chunkMemState.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


void init_chunks();
void init_chunkCreateReleaseZone( const IntVec2 &playerMPos_ );

ChunkZone &get_chunkCreateZoneRef();
ChunkZone &get_chunkReleaseZoneRef();

void chunks_debug();

Chunk &insert_and_init_new_chunk( chunkKey_t chunkKey_ );
void erase_from_chunks( chunkKey_t chunkKey_ );

MemMapEnt &get_memMapEntRef_in_activeChunk( const IntVec2 &anyMPos_ );


Chunk &get_chunkRef( chunkKey_t key_ );




void render_chunks();


}//---------------------- namespace: esrc -------------------------//
#endif

