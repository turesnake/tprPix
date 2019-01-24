/*
 * ========================= debug.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    MOST  POOR DEBUG SYSTEM
 * ----------------------------
 */
#ifndef _TPR_DEBUG_H_
#define _TPR_DEBUG_H_

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::flush;
using std::string;

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "Mesh.h"
#include "MapCoord.h"


namespace debug {//---------- namespace: debug --------------//


void init_debug();


//-- mapEntSlices --//
void clear_mapEntSlices();
void insert_new_mapEntSlice( const MapCoord &_mcpos );
void draw_renderPool_mapEntSlices();



}//-------------------- namespace: debug end --------------//



#endif 

