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

    esrc::animFrameSets.clear();

    esrc::animFrameSets.insert({"norman",  
                                AnimFrameSet{   "/norman/norman.P.png", 
                                                IntVec2{6, 2}, 12,
                                                true,
                                                true, std::vector<int>{12,8,8,12,8,8, 4,4,4,4,4,4} } });

    esrc::animFrameSets.insert({"bigMan",  
                                AnimFrameSet{   "/bigMan/bigMan.P.png", 
                                                IntVec2{6, 2}, 12,
                                                true,
                                                true, std::vector<int>{10,8,8,8,8,16, 3,6,3,3,6,3} } });

    esrc::animFrameSets.insert({"oakTree",  
                                AnimFrameSet{   "/oakTree/oakTree_2.P.png", 
                                                IntVec2{5, 4}, 17,
                                                true,
                                                false, std::vector<int>{} } });


    esrc::animFrameSets.insert({"lichen",  
                                AnimFrameSet{   "/lichen/lichen_2.P.png", 
                                                IntVec2{8, 4}, 32,
                                                false, //- 没有 shadow 数据
                                                false, std::vector<int>{} } });


    esrc::animFrameSets.insert({"wheat_Front",  
                                AnimFrameSet{   "/wheat/wheat_Front.P.png", 
                                                IntVec2{4, 1}, 4,
                                                false, //- 没有 shadow 数据
                                                false, std::vector<int>{} } });
    esrc::animFrameSets.insert({"wheat_Back",  
                                AnimFrameSet{   "/wheat/wheat_Back.P.png", 
                                                IntVec2{4, 1}, 4,
                                                false, //- 没有 shadow 数据
                                                false, std::vector<int>{} } });



    //---------------------------------//
    //     init all animFrameSets
    //---------------------------------//
    for( auto &pairRef : esrc::animFrameSets ){
        pairRef.second.init();
        //pairRef.second.debug();
    }



}



}//---------------------- namespace: esrc -------------------------//

