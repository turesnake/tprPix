/*
 * ========================= esrc_colliEntSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_COLLI_ENT_SET_H_
#define _TPR_ESRC_COLLI_ENT_SET_H_

//-------------------- CPP --------------------//
#include <unordered_map>
#include <string>

//-------------------- Engine --------------------//
#include "ColliEntSet.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//

inline std::unordered_map<int, ColliEntSet> colliEntSets {};
void load_colliEntSets();
void debug_colliEntSets(); //- debug

//-- 正反表 --
inline std::unordered_map<std::string, int> colliEntSet_name_idx {};
inline std::unordered_map<int, std::string> colliEntSet_idx_name {};
void init_colliEntSet_tables(); 


}//---------------------- namespace: esrc -------------------------//
#endif

