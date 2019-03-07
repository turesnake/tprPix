/*
 * ====================== chunkKey.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Chunk "id":  (int)w + (int)h
 * ----------------------------
 */
#include "chunkKey.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "sectionKey.h"


//#include "debug.h"


/* ===========================================================
 *                get_chunkIdx_in_section
 * -----------------------------------------------------------
 * 获得 目标chunk 在其 section 中的 idx [left-bottom]
 * ------
 * param: _mpos -- 任意 mapent 的 mpos
 */
size_t get_chunkIdx_in_section( const IntVec2 &_anyMPos ){

    IntVec2 mposOff = anyMPos_2_chunkMPos(_anyMPos) - anyMPos_2_sectionMPos(_anyMPos);
    int w = mposOff.x/ENTS_PER_CHUNK;
    int h = mposOff.y/ENTS_PER_CHUNK;
        assert( (w>=0) && (w<CHUNKS_PER_SECTION) &&
                (h>=0) && (h<CHUNKS_PER_SECTION) ); //- tmp
    return (size_t)(h*CHUNKS_PER_SECTION + w);
}








