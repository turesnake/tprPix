/*
 * ========================= esrc_chunkData.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_CHUNK_DATA_H_
#define _TPR_ESRC_CHUNK_DATA_H_

//-------------------- Engine --------------------//
#include "ChunkData.h"
#include "chunkKey.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

ChunkData *atom_insert_new_chunkData( chunkKey_t _chunkKey );
ChunkData *atom_get_chunkDataPtr( chunkKey_t _chunkKey );
void atom_erase_from_chunkDatas( chunkKey_t _chunkKey );

bool atom_find_is_chunkData_Done( chunkKey_t _chunkKey );
void atom_insert_2_chunkDataFlags( chunkKey_t _chunkKey );
void atom_erase_from_chunkDataFlags( chunkKey_t _chunkKey );


}//---------------------- namespace: esrc -------------------------//
#endif

