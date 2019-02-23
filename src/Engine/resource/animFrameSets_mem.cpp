/*
 * ================= animFrameSets_mem.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  animFrameSeta
 * -----
 *   这部分代码，应该放到 script层 中去
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "AnimFrameSet.h"
#include "srcs_engine.h" //- 所有资源

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

    animFrameSets.insert({"norman",  AnimFrameSet{ "/norman/norman.P.png", IntVec2{6, 2}, 12,
                                                    std::vector<int>{12,8,8,12,8,8, 4,4,4,4,4,4} } });

    animFrameSets.insert({"bigMan",  AnimFrameSet{ "/bigMan/bigMan.P.png", IntVec2{6, 2}, 12,
                                                    std::vector<int>{10,8,8,8,8,16, 3,6,3,3,6,3} } });

    animFrameSets.insert({"grass_1", AnimFrameSet{ "/grass/grass_1.P.png", IntVec2{6, 2}, 12,
                                                    std::vector<int>{8,8,8,8,8,8, 8,8,8,8,8,8} } });


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

