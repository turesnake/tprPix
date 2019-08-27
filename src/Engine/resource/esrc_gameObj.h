/*
 * ========================= esrc_gameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_GAME_OBJ_H
#define TPR_ESRC_GAME_OBJ_H

//-------------------- CPP --------------------//
#include <functional> 
#include <unordered_map>
#include <unordered_set> 

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "esrc_funcTypes.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

void init_gameObjs();

std::unordered_set<goid_t> &get_goids_active();
std::unordered_set<goid_t> &get_goids_inactive();

void insert_2_goids_active( goid_t id_ );
void insert_2_goids_inactive( goid_t id_ );


std::weak_ptr<GameObj> get_goWPtr( goid_t id_ );
GameObj &get_goRef( goid_t id_ );
GameObj *get_goRawPtr( goid_t id_ );


void foreach_goids_active( F_GOID_GOPTR fp_ );
void foreach_goids_inactive( F_GOID_GOPTR fp_ );

goid_t insert_new_regularGo( const IntVec2 mpos_, const IntVec2 pposOff_ );
goid_t insert_new_uiGo( const glm::dvec2 &basePointProportion_, const glm::dvec2 &offDPos_ );


void erase_the_go( goid_t id_ );
void insert_a_disk_gameObj( goid_t goid_, const IntVec2 mpos_, const IntVec2 pposOff_ );
void realloc_active_goes();
void realloc_inactive_goes();

}//---------------------- namespace: esrc -------------------------//
#endif

