/*
 * ========================= ssrc.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   脚本区 公共资源 存储地
 * ----------------------------
 */
#ifndef TPR_SRC_SCRIPT_H
#define TPR_SRC_SCRIPT_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set> 
#include <functional> 
#include <map>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "GameObjType.h"
#include "ParamBinary.h"


#include "Density.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"


class GameObj;
class UIObj;
//class json::GoJsonData;
//class json::UIGoJsonData;


namespace ssrc {//------------------ namespace: ssrc -------------------------//


goSpecId_t get_goSpecId( const std::string &name_ );

void insert_2_go_specId_names_containers( goSpecId_t id_, const std::string &name_ );


//--- 下面这部分 放的很乱... ---

//-- map自动生成器 使用的 goInit函数 ---
using F_GO_INIT = std::function<void(   GameObj&,
                                        const ParamBinary &dyParams_ )>;


void call_goInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const ParamBinary &dyParams_  );


bool find_from_goInit_funcs( goSpecId_t goSpecId_ );


void insert_2_goInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ );

void insert_2_go_jsonDatas( const json::GoJsonData &goJsonData_ );


const json::GoJsonData &get_goJsonData( goSpecId_t id_ );


//-------------------------------//
//             ui
//-------------------------------//
void insert_2_uiGo_specId_names_containers( goSpecId_t id_, const std::string &name_ );


//--- 下面这部分 放的很乱... ---
//-- map自动生成器 使用的 uiInit函数 ---
using F_UI_INIT = std::function<void(   UIObj*,
                                        const glm::vec2 & )>;


void insert_2_uiGo_jsonDatas( const json::UIGoJsonData &uiGoJsonData_ );


const json::UIGoJsonData &get_uiGoJsonData( goSpecId_t id_ );


bool find_from_uiGoInit_funcs( goSpecId_t goSpecId_ );


void call_uiGoInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const ParamBinary &dyParams_  );


void insert_2_uiGoInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ );

goSpecId_t get_uiGoSpecId( const std::string &name_ );


void clear_uiInit_funcs();
void clear_goInit_funcs();
void clear_uiGoInit_funcs();
void clear_go_specId_names();
void clear_go_name_specIds();
void clear_uiGo_specId_names();
void clear_uiGo_name_specIds();



}//---------------------- namespace: ssrc -------------------------//
#endif 

