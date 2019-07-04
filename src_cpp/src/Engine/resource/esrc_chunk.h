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


namespace esrc {//------------------ namespace: esrc -------------------------//


Chunk &insert_and_init_new_chunk(const IntVec2 &anyMPos_, ShaderProgram *sp_ );

MemMapEnt *get_memMapEntPtr( const MapCoord &anyMCpos_ );
MemMapEnt *get_memMapEntPtr( const IntVec2 &anyMPos_ );
bool find_from_chunks( chunkKey_t chunkKey_ );
Chunk &get_chunkRef( chunkKey_t key_ );

void render_chunks();


}//---------------------- namespace: esrc -------------------------//
#endif

