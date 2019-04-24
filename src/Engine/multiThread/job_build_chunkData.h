/*
 * ========================== job_build_chunkData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_JOB_BUILD_CHUNK_DATA_H_
#define _TPR_JOB_BUILD_CHUNK_DATA_H_

//-------------------- Engine --------------------//
#include "Job.h"
#include "chunkKey.h"


//-- job.argBinary --
struct ArgBinary_Build_ChunkData{
    chunkKey_t  chunkKey {};
};


#endif 

