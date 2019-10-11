/*
 * ===================== ssrc_gameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SSRC_GAME_OBJ_H
#define TPR_SSRC_GAME_OBJ_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/GameObjSpec.h"
#include "Script/resource/ssrc_oth.h"
#include "Script/json/json_all.h"

namespace ssrc {//------------------ namespace: ssrc -------------------------//


GameObjSpec &create_new_goSpec( goSpecId_t id_ )noexcept;
const GameObjSpec &get_goSpecRef( goSpecId_t id_ )noexcept; // support multi-thread
GameObjSpec &getnc_goSpecRef( goSpecId_t id_ )noexcept;


void insert_2_go_specId_names_containers( goSpecId_t id_, const std::string &name_ );

goSpecId_t str_2_goSpecId( const std::string &name_ );



bool find_from_goInit_funcs( goSpecId_t goSpecId_ );

void call_goInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const DyParam &dyParams_  );

void insert_2_goInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ );


}//---------------------- namespace: ssrc -------------------------//
#endif 

