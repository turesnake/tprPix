/*
 * ========================= ssrc.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   脚本区 公共资源 存储地
 * ----------------------------
 */
#ifndef _TPR_SRC_SCRIPT_H_
#define _TPR_SRC_SCRIPT_H_

//--- glm - 0.9.8 ---
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


namespace ssrc{ //------------------ namespace: ssrc -------------------------//

//-- 正反表 --
inline std::unordered_map<u32_t, std::string> acionHdle_typeId_names;
inline std::unordered_map<std::string, u32_t> acionHdle_name_typeIds;


//-------------------------------//
//             go
//-------------------------------//

//-- 正反表 --
inline std::unordered_map<goSpecId_t, std::string> go_specId_names;
inline std::unordered_map<std::string, goSpecId_t> go_name_specIds;


inline goSpecId_t get_goSpecId( const std::string &_name ){
        assert( ssrc::go_name_specIds.find(_name) != ssrc::go_name_specIds.end() );
    return ssrc::go_name_specIds.at(_name);
}


//--- 下面这部分 放的很乱... ---

//-- map自动生成器 使用的 goInit函数 ---
using F_GO_INIT = std::function<void( GameObj*,
                                        const IntVec2 &,
					                    float _fieldWeight,
					                    const MapAltitude &,
					                    const Density & )>;

inline std::unordered_map<goSpecId_t, F_GO_INIT> goInit_funcs; 

inline bool find_from_goInit_funcs( goSpecId_t _goSpecId ){
    return (ssrc::goInit_funcs.find(_goSpecId) != ssrc::goInit_funcs.end());
}


//-------------------------------//
//             ui
//-------------------------------//
//-- 正反表 --
inline std::unordered_map<uiObjSpecId_t, std::string> ui_specId_names;
inline std::unordered_map<std::string, uiObjSpecId_t> ui_name_specIds;


inline uiObjSpecId_t get_uiSpecId( const std::string &_name ){
        assert( ssrc::ui_name_specIds.find(_name) != ssrc::ui_name_specIds.end() );
    return ssrc::ui_name_specIds.at(_name);
}

//--- 下面这部分 放的很乱... ---
//-- map自动生成器 使用的 uiInit函数 ---
using F_UI_INIT = std::function<void(   UIObj*,
                                        const glm::vec2 & )>;

inline std::unordered_map<uiObjSpecId_t, F_UI_INIT> uiInit_funcs; 

inline bool find_from_uiInit_funcs( uiObjSpecId_t _uiSpecId ){
    return (ssrc::uiInit_funcs.find(_uiSpecId) != ssrc::uiInit_funcs.end());
}



}//---------------------- namespace: ssrc -------------------------//
#endif 

