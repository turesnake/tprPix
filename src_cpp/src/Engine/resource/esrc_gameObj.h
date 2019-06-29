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



std::unordered_set<goid_t> &get_goids_active();
std::unordered_set<goid_t> &get_goids_inactive();


std::weak_ptr<GameObj> get_goWPtr( goid_t id_ );
GameObj &get_goRef( goid_t id_ );
GameObj *get_goRawPtr( goid_t id_ );


void foreach_goids_active( F_GOID_GOPTR fp_ );
void foreach_goids_inactive( F_GOID_GOPTR fp_ );

goid_t insert_new_gameObj();
void insert_a_disk_gameObj( goid_t goid_ );
void realloc_active_goes();
void realloc_inactive_goes();
void signUp_newGO_to_mapEnt( GameObj &goRef_ );



}//---------------------- namespace: esrc -------------------------//
#endif

