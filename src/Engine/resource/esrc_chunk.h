/*
 * ========================= esrc_chunk.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_CHUNK_H_
#define _TPR_ESRC_CHUNK_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
//#include <set>

//-------------------- Engine --------------------//
#include "Chunk.h" 
#include "chunkKey.h"
#include "IntVec.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


Chunk *insert_and_init_new_chunk(const IntVec2 &_anyMPos, ShaderProgram *_sp );

MemMapEnt *get_memMapEntPtr( const MapCoord &_anyMCpos );
MemMapEnt *get_memMapEntPtr( const IntVec2 &_anyMPos );
bool find_from_chunks( chunkKey_t _chunkKey );
Chunk *get_chunkPtr( chunkKey_t _key );

void render_chunks();


}//---------------------- namespace: esrc -------------------------//
#endif

