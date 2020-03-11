/*
 * ====================== esrc_Job_Chunk.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_JOB_CHUNK_H
#define TPR_ESRC_JOB_CHUNK_H

#include <optional>

//-------------------- Engine --------------------//
#include "Job_Chunk.h"
#include "chunkKey.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_job_chunks();

Job_Chunk &atom_insert_new_job_chunk( chunkKey_t chunkKey_ );
void atom_erase_from_job_chunks( chunkKey_t chunkKey_ );

std::optional<Job_Chunk*> atom_getnc_job_chunk_ptr( chunkKey_t chunkKey_ );


void atom_push_back_2_job_chunkFlags( chunkKey_t chunkKey_ );
std::optional<chunkKey_t> atom_pop_from_job_chunkFlags();


}//---------------------- namespace: esrc -------------------------//
#endif

