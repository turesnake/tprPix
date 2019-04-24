/*
 * ========================= esrc_chunk.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_CHUNK_H_
#define _TPR_ESRC_CHUNK_H_

//-------------------- CPP --------------------//
#include <deque>

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




//----------------------------------
//          tmp
// 在未来，会被 多线程chunk生成器 取代
//     所以暂时不需要将其 atom 化
// 将需要生成的 chunk 暂存入容器，逐个生成

inline std::deque<chunkKey_t> chunksDeque {};

inline void push_to_chunksDeque( chunkKey_t _chunkKey ){
    esrc::chunksDeque.push_back( _chunkKey );
}

inline bool is_chunksDeque_empty(){
    return esrc::chunksDeque.empty();
}

inline chunkKey_t pop_from_chunksDeque(){
    assert( !esrc::chunksDeque.empty() );
    chunkKey_t key = esrc::chunksDeque.at(0);
    esrc::chunksDeque.pop_front();
    return key;
}


}//---------------------- namespace: esrc -------------------------//
#endif

