/*
 * ========================= gpgpu_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
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
    esrc::pixGpgpu.add_new_uniform( "chunkCPosX" ); //- int
    esrc::pixGpgpu.add_new_uniform( "chunkCPosY" ); //- int
    esrc::pixGpgpu.add_new_uniform( "altiSeed" );   //- float

    esrc::pixGpgpu.add_new_uniform( "seaLvl_leftBottom" );   //- float
    esrc::pixGpgpu.add_new_uniform( "seaLvl_rightBottom" );   //- float
    esrc::pixGpgpu.add_new_uniform( "seaLvl_leftTop" );   //- float
    esrc::pixGpgpu.add_new_uniform( "seaLvl_rightTop" );   //- float


    /*
    esrc::pixGpgpu.add_new_uniform( "freq3" ); 
    esrc::pixGpgpu.add_new_uniform( "freq4" ); 

    esrc::pixGpgpu.add_new_uniform( "seed1" ); 
    esrc::pixGpgpu.add_new_uniform( "seed2" ); 
    esrc::pixGpgpu.add_new_uniform( "seed3" ); 
    esrc::pixGpgpu.add_new_uniform( "seed4" ); 
    */
    //...
    esrc::pixGpgpu.release();
}




}//---------------------- namespace: esrc -------------------------//

