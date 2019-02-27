/*
 * =================== SectionChunkSet.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section "二级距离场"集 
 * ----------------------------
 */
#include "SectionChunkSet.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "random.h"


namespace{//----------- namespace ---------------//


    std::default_random_engine  randEngine; //-随机数引擎实例
    std::uniform_int_distribution<int> uDistribution(0,
                                        MapChunk::pixSideLen-1); 
                                        // [0,19] 

}//-------------- namespace : end ---------------//




/* ===========================================================
 *                  init
 * -----------------------------------------------------------
 * param: _mpos -- 这个 section 中的任意 mapent.mpos 
 */
void SectionChunkSet::init( const IntVec2 &_mpos ){

    //-------------------------------//
    // 每初始化一个 section，就重分配一个 seed
    // 但这个 section内部。则使用这个 连续的 引擎
    //       (临时简易方案...)
    //-------------------------------// 
    randEngine.seed(get_new_seed());

    //-------------------------------//
    //          sectionKey
    //-------------------------------//
    //-- “地板除法，向低取节点值”, 再乘回 节点间距。
    //   获得 所在section 左下ent mpos
    IntVec2 mpos = get_section_mpos( _mpos );
    mcpos.set_by_mpos( mpos );
    sectionKey.init_by_mapEnt_mpos( mcpos.get_mpos() ); 
                    //-- 这里的计算重复了，但问题不大。

    //-------------------------------//
    //  制作 64*64 个 距离场
    //  填入 fields 容器中
    //-------------------------------//
    MapChunk   chunk  {};
    IntVec2    lPPos  {}; 
    for( int h=0; h<chunkSideLen; h++ ){  //- each chunk
        for( int w=0; w<chunkSideLen; w++ ){
            //--- chunk.mcpos ---
            chunk.mcpos.set_by_mpos(mcpos.get_mpos().x + w*MapChunk::entSideLen,
                                    mcpos.get_mpos().y + h*MapChunk::entSideLen);
            //--- chunk.nodePPos ---
            lPPos.set(  uDistribution(randEngine), 
                        uDistribution(randEngine) );
            chunk.nodePPos = chunk.mcpos.get_ppos() + lPPos;
            //--- chunk.lNodePPosOff ---
            chunk.lNodePPosOff = chunk.nodePPos - this->mcpos.get_ppos();
            //--- chunk.isLand      暂空置
            //--- chunk.ecoSysType  暂空置
            chunks.push_back( chunk ); //- copy
        }
    }
}



/* ===========================================================
 *                get_mapChunkPtr_by_mpos
 * -----------------------------------------------------------
 * 传入本section中的任意一个 mpos，获得其对应的 MapChunk 的 访问权 
 * param: _mpos -- 这个 section 中的任意 mapent.mpos 
 */
MapChunk *SectionChunkSet::get_mapChunkPtr_by_mpos( const IntVec2 &_mpos ){

    IntVec2  lMPosOff = _mpos - mcpos.get_mpos(); 
    IntVec2  chunkWH_ = floorDiv( lMPosOff, (float)chunkSideLen );
    size_t idx = chunkWH_.y*chunkSideLen + chunkWH_.x;
        assert( (idx>=0) && (idx<chunks.size()) ); //- tmp
    return (MapChunk*)&(chunks.at(idx));
}



