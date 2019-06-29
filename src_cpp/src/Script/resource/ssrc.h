/*
 * ========================= ssrc.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   脚本区 公共资源 存储地
 * ----------------------------
 */
#ifndef TPR_SRC_SCRIPT_H_
#define TPR_SRC_SCRIPT_H_

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
#include "UIObjType.h"


#include "MapAltitude.h"
#include "Density.h"

class GameObj;
class UIObj;


namespace ssrc {//------------------ namespace: ssrc -------------------------//


goSpecId_t get_goSpecId( const std::string &name_ );

void insert_2_go_specId_names_containers( goSpecId_t id_, const std::string &name_ );


//--- 下面这部分 放的很乱... ---

//-- map自动生成器 使用的 goInit函数 ---
using F_GO_INIT = std::function<void(   GameObj&,
                                        const IntVec2 &,
					                    float,
					                    const MapAltitude &,
					                    const Density & )>;


void call_goInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const IntVec2 &mpos_,
					    float fieldWeight_,
					    const MapAltitude &alti_,
					    const Density &density_  );


bool find_from_goInit_funcs( goSpecId_t goSpecId_ );

void insert_2_goInit_funcs( goSpecId_t goSpecId_,
                            const F_GO_INIT &functor_ );


//-------------------------------//
//             ui
//-------------------------------//

uiObjSpecId_t get_uiSpecId( const std::string &name_ );

void insert_2_ui_specId_names_containers(   uiObjSpecId_t id_, 
                                            const std::string &name_ );


//--- 下面这部分 放的很乱... ---
//-- map自动生成器 使用的 uiInit函数 ---
using F_UI_INIT = std::function<void(   UIObj*,
                                        const glm::vec2 & )>;

bool find_from_uiInit_funcs( uiObjSpecId_t uiSpecId_ );

void call_uiInit_func(  uiObjSpecId_t id_,  
                        UIObj *uiObjPtr_,
                        const glm::vec2 &fpos_ );



void insert_2_uiInit_funcs( uiObjSpecId_t id_,
                            const F_UI_INIT &functor_ );



void clear_uiInit_funcs();
void clear_goInit_funcs();
void clear_go_specId_names();
void clear_go_name_specIds();
void clear_ui_specId_names();
void clear_ui_name_specIds();



}//---------------------- namespace: ssrc -------------------------//
#endif 

