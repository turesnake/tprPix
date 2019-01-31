/*
 * ================= animFrameSets_mem.cpp =====================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 
 * ----------------------------------------------------------
 *  资源管理:  animFrameSeta
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "AnimFrameSet.h"
#include "srcs_engine.h" //- 所有资源


using std::string;


//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//

/* ===========================================================
 *                     load_animFrameSets
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 AnimFrameSet 资源
 */
void load_animFrameSets(){

    animFrameSets.clear();

    animFrameSets.insert( {"human_new",      AnimFrameSet{ "/human/human_new.P.png",             IntVec2{15, 30}, IntVec2{4, 1}, 4 } } );
    animFrameSets.insert( {"singleFootWalk", AnimFrameSet{ "/singleFoot/singleFootWalk_1.P.png", IntVec2{26, 29}, IntVec2{4, 2}, 8 } } );
    animFrameSets.insert( {"norman",         AnimFrameSet{ "/norman/norman.P.png",               IntVec2{24, 39}, IntVec2{6, 2}, 12 } } );
    animFrameSets.insert( {"bigMan",         AnimFrameSet{ "/bigMan/bigMan.P.png",               IntVec2{22, 40}, IntVec2{6, 2}, 12 } } );



    //---------------------------------//
    //        init all animFrameSets
    //---------------------------------//
    auto it = animFrameSets.begin();
    for( ; it!=animFrameSets.end(); it++ ){
        it->second.init();
        //it->second.debug();
    }
}



}//---------------------- namespace: esrc -------------------------//

