/*
 * ====================== GameSeed.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   game seed for random
 * ----------------------------
 */
#include "GameSeed.h"


#include "debug.h"


/* ===========================================================
 *                        init 
 * -----------------------------------------------------------
 */
void GameSeed::init(){

    baseSeed = get_new_seed();
    //----
        //cout << "baseSeed = " << baseSeed << endl;

    std::default_random_engine dre;
    dre.seed( baseSeed );

    std::uniform_int_distribution<int> udi( 0,9 ); //-[0,9]

    /*
    for( int i=0; i<20; i++ ){
        cout<< (u32_t)udi(dre) << ", ";
    }
        cout << endl;
    */

    


}


