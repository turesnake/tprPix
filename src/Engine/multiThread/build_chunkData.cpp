/*
 * =================== build_chunkData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  job: build chunkData
 * ----------------------------
 */

//-------------------- C --------------------//
#include <cassert>
#include <cstring>

//-------------------- CPP --------------------//


//-------------------- Engine --------------------//
#include "jobs_all.h"
#include "Job.h"
#include "job_build_chunkData.h"



/* ===========================================================
 *                build_chunkData_main
 * -----------------------------------------------------------
 */
void build_chunkData_main( const Job &_job ){

    //-------------------//
    //   job.argBinary
    //-------------------//
    assert( _job.argBinary.size() == sizeof(ArgBinary_Build_ChunkData) );
    ArgBinary_Build_ChunkData arg;
    memcpy( (void*)&arg,
            (void*)&(_job.argBinary.at(0)),
            sizeof(ArgBinary_Build_ChunkData) );

    



        





}







