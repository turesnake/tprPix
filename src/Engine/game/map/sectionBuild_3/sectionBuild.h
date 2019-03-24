/*
 * =================== sectionBuild.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section build api  [3th]
 * ----------------------------
 */
#ifndef _TPR_SECTION_BUILD_H
#define _TPR_SECTION_BUILD_H

//------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "IntVec.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//
    

void init();

void build_9_chunks_3( const IntVec2 &_playerMPos );
void build_new_chunk_in_update_3();



}//----------------- namespace: sectionBuild: end -------------------//
#endif

