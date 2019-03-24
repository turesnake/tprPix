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
#include <math.h>

//-------------------- Engine --------------------//
#include "config.h"
#include "random.h"
#include "PerlinNoise3D.h" //- out 
#include "srcs_engine.h"

//#include "debug.h"


//namespace{//-------- namespace: --------------//
//}//------------- namespace: end --------------//

/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 * -- 
 */
void EcoSysInMap::init(){

    //------------------//
    //     weight
    //------------------//
    // 4*4 个 ecosysinmap 组成一个 perlinEnt
    float freq = 1.0 / 4.0; 
    IntVec2 v = floorDiv(mcpos.get_mpos(), ENTS_PER_SECTION);
    this->weight = esrc::gameSeed.pn_ecoSysInMap.noise( v.x * freq, 
                                                        v.y * freq, 
                                                        0.1) * 100.0; 
                                                        //- [-100.0, 100.0]
    //------------------//
    //   occupyWeight
    //------------------//
    this->init_occupyWeight();


    //- 为每个 ecosys 随机分配一个 type (临时方案) --
    this->ecoSysType = get_rand_EcoSysType();
    //...

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
    //...

    //---------------------------//
    //          
    //---------------------------//

    //-- 计算 chunkTotalNum --
    //-- 找出 占有chunk数 最多的 quad --
    /*
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
    */

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



/* ===========================================================
 *                 init_occupyWeight
 * -----------------------------------------------------------
 */
void EcoSysInMap::init_occupyWeight(){

    //- 以 section 为单位的 坐标 --
    IntVec2 SPos = floorDiv( this->get_mpos(), ENTS_PER_SECTION );

    //-- 本 ecosys mpos 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2 oddEven = floorMod( SPos, 2 );

    //-- 相邻 ecosysinmap 间的 occupyWeight 没有关联性，就是 白噪音 --
    float Fidx = esrc::gameSeed.pn_ecoSysInMap.noise(SPos.x, SPos.y, 0.2) * 50.0 + 100.0; //- [50.0, 150.0]
    assert( Fidx > 0 );
    size_t randIdx = (size_t)floor(Fidx); //- [50, 150]

    this->occupyWeight = get_occupyWeight( oddEven, randIdx );
}












