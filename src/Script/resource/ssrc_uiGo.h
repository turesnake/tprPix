/*
 * ======================= ssrc_uiGo.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SSRC_UIGO_H
#define TPR_SSRC_UIGO_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//--------------- Script ------------------//
#include "Script/resource/ssrc_oth.h"
#include "Script/json/json_all.h"


namespace ssrc {//------------------ namespace: ssrc -------------------------//



void insert_2_uiGo_specId_names_containers( goSpecId_t id_, const std::string &name_ );

void insert_2_uiGo_jsonDatas( const json::UIGoJsonData &uiGoJsonData_ );

const json::UIGoJsonData &get_uiGoJsonData( goSpecId_t id_ );

bool find_from_uiGoInit_funcs( goSpecId_t goSpecId_ );

void call_uiGoInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const DyParam &dyParams_  );

void insert_2_uiGoInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ );

goSpecId_t get_uiGoSpecId( const std::string &name_ );



}//---------------------- namespace: ssrc -------------------------//
#endif 

