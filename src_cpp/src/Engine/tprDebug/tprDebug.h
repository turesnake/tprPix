/*
 * ========================= tprDebug.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    MOST  POOR DEBUG SYSTEM
 * ----------------------------
 */
#ifndef TPR_DEBUG_H
#define TPR_DEBUG_H

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


namespace tprDebug {//---------- namespace: tprDebug --------------//

void init_debug();

//-- mapEntSlices --//
void clear_mapEntSlices();
void insert_new_mapEntSlice( const MapCoord &mcpos_ );
void draw_renderPool_mapEntSlices();

void clear_pointPics();
void insert_new_pointPic( const glm::vec2 &fpos_ );
void draw_renderPool_pointPics();


}//-------------------- namespace: tprDebug end --------------//



#endif 

