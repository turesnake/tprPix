/*
 * ========================== jobs_all.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOBS_ALL_H
#define TPR_JOBS_ALL_H

//-------------------- Engine --------------------//
#include "Job.h"
#include "chunkKey.h"


//-- job.argBinary --
struct ArgBinary_Build_ChunkData{
    chunkKey_t  chunkKey {};
};

void build_chunkData_main( const Job &job_ );




#endif 

