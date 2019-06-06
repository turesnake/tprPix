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

//--- glm - - 0.9.9.5 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set> 
#include <functional> 
#include <map>

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

        //  和 esrc 一样，这里的很多数据，应该放到 .cpp 中去 

//-- 正反表 --
//inline std::unordered_map<u32_t, std::string> acionHdle_typeId_names;
//inline std::unordered_map<std::string, u32_t> acionHdle_name_typeIds;


//-------------------------------//
//             go
//-------------------------------//

//-- 正反表 --
//inline std::unordered_map<goSpecId_t, std::string> go_specId_names;
//inline std::unordered_map<std::string, goSpecId_t> go_name_specIds;

goSpecId_t get_goSpecId( const std::string &_name );

void insert_2_go_specId_names_containers( goSpecId_t _id, const std::string &_name );


//--- 下面这部分 放的很乱... ---

//-- map自动生成器 使用的 goInit函数 ---
using F_GO_INIT = std::function<void( GameObj*,
                                        const IntVec2 &,
					                    float,
					                    const MapAltitude &,
					                    const Density & )>;

//inline std::unordered_map<goSpecId_t, F_GO_INIT> goInit_funcs; 

void call_goInit_func(  goSpecId_t _id,
                        GameObj *_goPtr,
                        const IntVec2 &_mpos,
					    float _fieldWeight,
					    const MapAltitude &_alti,
					    const Density &_density  );


bool find_from_goInit_funcs( goSpecId_t _goSpecId );

void insert_2_goInit_funcs( goSpecId_t _goSpecId,
                            const F_GO_INIT &_functor );


//-------------------------------//
//             ui
//-------------------------------//
//-- 正反表 --
//inline std::unordered_map<uiObjSpecId_t, std::string> ui_specId_names;
//inline std::unordered_map<std::string, uiObjSpecId_t> ui_name_specIds;

uiObjSpecId_t get_uiSpecId( const std::string &_name );

void insert_2_ui_specId_names_containers(   uiObjSpecId_t _id, 
                                            const std::string &_name );


//--- 下面这部分 放的很乱... ---
//-- map自动生成器 使用的 uiInit函数 ---
using F_UI_INIT = std::function<void(   UIObj*,
                                        const glm::vec2 & )>;

//inline std::unordered_map<uiObjSpecId_t, F_UI_INIT> uiInit_funcs; 

/*
inline bool find_from_uiInit_funcs( uiObjSpecId_t _uiSpecId ){
    return (ssrc::uiInit_funcs.find(_uiSpecId) != ssrc::uiInit_funcs.end());
}
*/

bool find_from_uiInit_funcs( uiObjSpecId_t _uiSpecId );

void call_uiInit_func(  uiObjSpecId_t _id,  
                        UIObj *_uiObjPtr,
                        const glm::vec2 &_fpos );



void insert_2_uiInit_funcs( uiObjSpecId_t _id,
                            const F_UI_INIT &_functor );



void clear_uiInit_funcs();
void clear_goInit_funcs();
void clear_go_specId_names();
void clear_go_name_specIds();
void clear_ui_specId_names();
void clear_ui_name_specIds();



}//---------------------- namespace: ssrc -------------------------//
#endif 

