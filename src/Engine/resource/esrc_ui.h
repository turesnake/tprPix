/*
 * ========================= esrc_ui.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_UI_H_
#define _TPR_ESRC_UI_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <functional> 
#include <unordered_map>
#include <unordered_set> 


//-------------------- Engine --------------------//
#include "GameObj.h"
#include "esrc_funcTypes.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


//--- mem ---//
inline std::unordered_map<goid_t, GameObj> memUIs {}; //- 所有 ui实例 实际存储区。

                                    
inline std::unordered_set<goid_t> uiIds_active   {}; //- 激活组 (身处 激活圈 之内)



void foreach_memUIs( F_GOID_GOPTR _fp );
void foreach_uiIds_active( F_GOID_GOPTR _fp );


goid_t insert_new_ui();



inline GameObj *get_memUIPtr( goid_t _goid ){
        assert( memUIs.find(_goid) != memUIs.end() );//- tmp
    return  static_cast<GameObj*>( &(memUIs.at(_goid)) );
}




}//---------------------- namespace: esrc -------------------------//
#endif

