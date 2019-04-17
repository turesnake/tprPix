/*
 * ========================= srcs_script.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   脚本区 公共资源 存储地
 * ----------------------------
 */
#ifndef _TPR_SRC_SCRIPT_H_
#define _TPR_SRC_SCRIPT_H_

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
#include "GameObj.h" //- goSpecId_t
#include "Altitude.h"
#include "Density.h"


namespace ssrc{ //------------------ namespace: ssrc -------------------------//

//-- 正反表 --
inline std::unordered_map<u32_t, std::string> acionHdle_typeId_names;
inline std::unordered_map<std::string, u32_t> acionHdle_name_typeIds;

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
					                    const Altitude &,
					                    const Density & )>;

inline std::unordered_map<goSpecId_t, F_GO_INIT> goInit_funcs; 

inline bool find_from_goInit_funcs( goSpecId_t _goSpecId ){
    return (ssrc::goInit_funcs.find(_goSpecId) != ssrc::goInit_funcs.end());
}



}//---------------------- namespace: ssrc -------------------------//
#endif 

