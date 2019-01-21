/*
 * ========================= actions_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.23
 * ----------------------------------------------------------
 *  资源管理:  Action
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "Action.h"
#include "srcs_engine.h" //- 所有资源


using std::string;


//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//

/* ===========================================================
 *                     actions_load
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 Actions 资源
 */
void actions_load(){

    actions.clear();

    actions.insert( {"human_1",  Action{ "/human/human_1.P.png", IntVec2{13, 21}, IntVec2{4, 1}, 4 } } );




    //---------------------------------//
    //        init all actions
    //---------------------------------//
    auto it = actions.begin();
    for( ; it!=actions.end(); it++ ){
        it->second.init();
        //it->second.debug();
    }
}



}//---------------------- namespace: esrc -------------------------//

