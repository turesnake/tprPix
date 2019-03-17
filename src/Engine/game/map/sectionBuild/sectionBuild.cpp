/*
 * =================== sectionBuild.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#include "sectionBuild.h"
#include "sectionBuild_innerDatas.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void init(){

    randEngine.seed( get_new_seed() ); //- tmp
    

    //-- 填上所有序号 --
    randWH.clear();
    randWH.reserve( ENTS_PER_SECTION * ENTS_PER_SECTION );
    for( int h=0; h<ENTS_PER_SECTION; h++ ){
        for( int w=0; w<ENTS_PER_SECTION; w++ ){
            randWH.push_back(  IntVec2{ w, h } );
        }
    }

    //...

}




}//----------------- namespace: sectionBuild: end -------------------//

