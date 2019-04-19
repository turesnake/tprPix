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
#include <unordered_map>
#include <deque>

//-------------------- Engine --------------------//
#include "Chunk.h" 
#include "chunkKey.h"
#include "IntVec.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


//-- 可能在 mem态，加载很多张 chunk
//-- 但每一渲染帧，只会有 1／2／4 张 map，被渲染。
// key 为 chunk.chunkKey.key;
inline std::unordered_map<chunkKey_t, Chunk> chunks {};

Chunk *insert_new_chunk( const IntVec2 &_anyMPos );
MemMapEnt *get_memMapEntPtr( const MapCoord &_anyMCpos ); //- 临时放这 
MemMapEnt *get_memMapEntPtr( const IntVec2 &_anyMPos ); //- 临时放这 

inline bool find_from_chunks( chunkKey_t _chunkKey ){
    return (esrc::chunks.find(_chunkKey) != esrc::chunks.end());
}

inline Chunk *get_chunkPtr( chunkKey_t _key ){
        assert( chunks.find(_key) != chunks.end() );//- must exist
    return static_cast<Chunk*>( &(chunks.at(_key)) );
}
inline Chunk &get_chunkRef( chunkKey_t _key ){
        assert( chunks.find(_key) != chunks.end() );//- must exist
    return chunks.at(_key);
}

//----------------------------------
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

