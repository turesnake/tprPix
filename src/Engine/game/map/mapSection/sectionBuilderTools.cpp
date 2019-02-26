/*
 * ============ sectionBuilderTools.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   存放 section生成器的 通用工具
 * ----------------------------
 */
#include "sectionBuilderTools.h"


/* ===========================================================
 *             init_sectionBuilderTools
 * -----------------------------------------------------------
 * -- 
 */
void init_sectionBuilderTools(  const IntVec2 &_entWH,
                                const IntVec2 &_pixWH ){
    entWH = _entWH;
    pixWH = _pixWH;
    //--------------------------//
    //     初始化  randWH
    //--------------------------//
    randWH.clear();
    randWH.reserve( entWH.x * entWH.y );
    for( int h=0; h<entWH.y; h++ ){
        for( int w=0; w<entWH.x; w++ ){
            randWH.push_back(  IntVec2{ w, h } );
        }
    }
    //...

    //--------------------------//
    //     初始化  randEngine
    //--------------------------//
    // 完成态时，seed应该取自 GameSeed.
    // 目前暂时用 随机seed 替代 tmp
    randEngine.seed( get_new_seed() );

}


