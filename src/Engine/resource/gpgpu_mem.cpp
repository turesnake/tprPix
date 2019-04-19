/*
 * ========================= gpgpu_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- Engine --------------------//
#include "esrc_GPGPU.h"

#include "config.h"
#include "GPGPU.h"

//#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                init_gpgpus
 * -----------------------------------------------------------
 */
void init_gpgpus(){

    //------------------//
    //     pixGpgpu
    //------------------//
    esrc::pixGpgpu.init( IntVec2{ PIXES_PER_CHUNK, PIXES_PER_CHUNK },
                        "/pixGpgpu.vs",
                        "/pixGpgpu.fs" );

    esrc::pixGpgpu.add_new_uniform( "chunkCFPos" ); //- 2-float

    esrc::pixGpgpu.add_new_uniform( "altiSeed_pposOffSeaLvl" );  //- 2-float
    esrc::pixGpgpu.add_new_uniform( "altiSeed_pposOffBig" );  //- 2-float
    esrc::pixGpgpu.add_new_uniform( "altiSeed_pposOffMid" );  //- 2-float
    esrc::pixGpgpu.add_new_uniform( "altiSeed_pposOffSml" );  //- 2-float


    //...
    esrc::pixGpgpu.release();
}




}//---------------------- namespace: esrc -------------------------//

