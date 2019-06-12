/*
 * ========================= esrc_gameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_GAME_OBJ_H_
#define TPR_ESRC_GAME_OBJ_H_

//-------------------- CPP --------------------//
#include <functional> 
#include <unordered_map>
#include <unordered_set> 

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "esrc_funcTypes.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


std::unordered_map<goid_t, GameObj> &get_memGameObjs();
std::unordered_set<goid_t> &get_goids_active();
std::unordered_set<goid_t> &get_goids_inactive();

void foreach_memGameObjs( F_GOID_GOPTR _fp );
void foreach_goids_active( F_GOID_GOPTR _fp );
void foreach_goids_inactive( F_GOID_GOPTR _fp );

goid_t insert_new_gameObj();
void insert_a_disk_gameObj( goid_t _goid );
void realloc_active_goes();
void realloc_inactive_goes();
void signUp_newGO_to_mapEnt( GameObj *_goPtr );

GameObj *get_memGameObjPtr( goid_t _goid );


}//---------------------- namespace: esrc -------------------------//
#endif

