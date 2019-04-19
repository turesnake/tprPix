/*
 * ========================= esrc_gameObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_GAME_OBJ_H_
#define _TPR_ESRC_GAME_OBJ_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <functional> 
#include <unordered_map>
#include <unordered_set> 


//-------------------- Engine --------------------//
#include "GameObj.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

using FUNC_V_V = std::function<void()>;
using F_GOID_GOPTR = std::function<void(goid_t,GameObj*)>;

//--- mem ---//
inline std::unordered_map<goid_t, GameObj> memGameObjs {}; //- 所有载入内存的 go实例 实际存储区。
                                    
inline std::unordered_set<goid_t> goids_active   {}; //- 激活组 (身处 激活圈 之内)
inline std::unordered_set<goid_t> goids_inactive {}; //- 未激活组 (身处 激活圈 之外)

inline FUNC_V_V  goSpecIds_SignUp  {nullptr}; //- goSpecIds 注册函数对象

void foreach_memGameObjs( F_GOID_GOPTR _fp );
void foreach_goids_active( F_GOID_GOPTR _fp );
void foreach_goids_inactive( F_GOID_GOPTR _fp );

goid_t insert_new_gameObj();
void realloc_active_goes();
void realloc_inactive_goes();
void signUp_newGO_to_mapEnt( GameObj *_goPtr );

inline GameObj *get_memGameObjPtr( goid_t _goid ){
        assert( memGameObjs.find(_goid) != memGameObjs.end() );//- tmp
    return  static_cast<GameObj*>( &(memGameObjs.at(_goid)) );
}


}//---------------------- namespace: esrc -------------------------//
#endif

