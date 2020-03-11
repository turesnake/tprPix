
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
#include <string>
#include <functional> 
#include <optional>
#include <unordered_map>
#include <unordered_set> 

//-------------------- Engine --------------------//
#include "GameObj.h"



namespace esrc {//------------------ namespace: esrc -------------------------//

void init_gameObjs();

void debug_for_gameObjs()noexcept;

std::unordered_set<goid_t> &get_goids_active();
std::unordered_set<goid_t> &get_goids_inactive();

void insert_2_goids_active( goid_t id_ );
void insert_2_goids_inactive( goid_t id_ );

std::optional<GameObj*> get_goPtr( goid_t id_ )noexcept;

//GameObj *get_goRawPtr( goid_t id_ );

bool is_go_active(goid_t id_  ); //- tmp

using F_GOID_GOPTR = std::function<void(goid_t, GameObj&)>;

void foreach_goids_active( F_GOID_GOPTR fp_ );
void foreach_goids_inactive( F_GOID_GOPTR fp_ );

goid_t insert_new_regularGo( const glm::dvec2 &dpos_, size_t goUWeight_ );
goid_t insert_new_uiGo( const glm::dvec2 &basePointProportion_, const glm::dvec2 &offDPos_, size_t goUWeight_ );

void insert_a_diskGo( goid_t goid_, const glm::dvec2 &dpos_, size_t goUWeight_ );
void erase_the_go( goid_t id_ );

void realloc_active_goes();
void realloc_inactive_goes();


void refresh_worldUIGo_chunkSignUpData( GameObj &goRef_, const glm::dvec2 &moveVec_ );


}//---------------------- namespace: esrc -------------------------//
#endif

