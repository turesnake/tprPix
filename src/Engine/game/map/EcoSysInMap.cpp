/*
 * ========================== EcoSysInMap.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#include "EcoSysInMap.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- C --------------------//
//#include <set>
//#include <algorithm>

//-------------------- Engine --------------------//
#include "config.h"

#include "debug.h"


namespace{//-------- namespace: --------------//

    //- 4个 section 坐标偏移（以 ENTS_PER_SECTION 为单位）
    std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ -1, -1 },
        IntVec2{  0, -1 },
        IntVec2{ -1,  0 },
        IntVec2{  0,  0 }
    };



}//------------- namespace: end --------------//

/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 * -- 
 */
void EcoSysInMap::init(){

    //- 为每个 ecosys 随机分配一个 type (临时方案) --
    ecoSysType = get_rand_EcoSysType();
    //...
    init_quadSectionKeys();
}



/* ===========================================================
 *                init_quadSectionKeys
 * -----------------------------------------------------------
 * -- 填充 quadSectionKeys
 */
void EcoSysInMap::init_quadSectionKeys(){
    if( this->is_quadSectionKeys_set ){
        return;
    }
    //-----
    IntVec2        mpos = mcpos.get_mpos();
    IntVec2        tmpMPos;
    sectionKey_t   tmpKey;
    quadSectionKeys.clear();
    for( const auto &whOff : quadSectionKeyOffs ){
        tmpMPos.x = mpos.x + whOff.x*ENTS_PER_SECTION;
        tmpMPos.y = mpos.y + whOff.y*ENTS_PER_SECTION;
        tmpKey = sectionMPos_2_sectionKey( tmpMPos );
        //---
        quadSectionKeys.push_back( tmpKey );//- copy
    }
    assert( quadSectionKeys.size() == 4 );
    this->is_quadSectionKeys_set = true;
}



/* ===========================================================
 *                       plan
 * -----------------------------------------------------------
 * -- 在 section 阶段的 准备性数据制作完毕后，调用此函数来 做 生态规划
 */
void EcoSysInMap::plan(){
    if( this->is_plan_done ){
        return;
    }


    //---------------------------//
    //          check
    //---------------------------//
    assert( this->is_quadSectionKeys_set );
    assert( this->is_all_sections_done.is_all_true() );


    //---------------------------//
    //          
    //---------------------------//

    //-- 计算 chunkTotalNum --
    //-- 找出 占有chunk数 最多的 quad --
    int tmpNum;
    int mostQuadIdx = 0; //- 占有 chunk数最多的quad 的 idx。
    int mostQuadChunkNum = 0;
    chunkTotalNum = 0;
    for( int i=0; i<QUAD_NUM; i++ ){
        tmpNum = chunkNums_per_quad.at(i);
        if( tmpNum > mostQuadChunkNum ){
            mostQuadChunkNum = tmpNum;
            mostQuadIdx = i;
        }
    }
    //...


    // -- 规划阶段完成的工作：--
    // 确定每个 chunk 在 本ecosys 中的 “等级-类型”
    // 并将这个信息 反向写到 chunk 中
    // 这样，当一个 chunk开始创建自己的数据时，将根据这些数 已经被设置好的信息
    // 正式生成自己。
    // ---
    // 更进一步的 规划，甚至可以直接规定，目标 chunk 中将出现一个 XX预制物。
    //
    //
    



    this->is_plan_done = true;
}

