/*
 * =================== ChunkFieldSet.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk "二级距离场"集 
 * ----------------------------
 */
#include "ChunkFieldSet.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"


namespace{//----------- namespace ---------------//


    std::default_random_engine  randEngine; //-随机数引擎实例
    std::uniform_int_distribution<int> uDistribution(0,
                                        MapField::pixSideLen-1); 
                                        // [0,19] 

}//-------------- namespace : end ---------------//




/* ===========================================================
 *                  init
 * -----------------------------------------------------------
 * param: _mpos -- 这个 chunk 中的任意 mapent.mpos 
 */
void ChunkFieldSet::init( const IntVec2 &_mpos ){

    //-------------------------------//
    // 每初始化一个 chunk，就重分配一个 seed
    // 但这个 chunk内部。则使用这个 连续的 引擎
    //       (临时简易方案...)
    //-------------------------------// 
    randEngine.seed(get_new_seed());

    //-------------------------------//
    //          chunkKey
    //-------------------------------//
    //-- “地板除法，向低取节点值”, 再乘回 节点间距。
    //   获得 所在chunk 左下ent mpos
    IntVec2 mpos = get_chunk_mpos( _mpos );
    mcpos.set_by_mpos( mpos );
    chunkKey.init_by_mapEnt_mpos( mcpos.get_mpos() ); 
                    //-- 这里的计算重复了，但问题不大。

    //-------------------------------//
    //  制作 64*64 个 距离场
    //  填入 fields 容器中
    //-------------------------------//
    MapField   field  {};
    IntVec2    lPPos  {}; 
    for( int h=0; h<fieldSideLen; h++ ){  //- each field
        for( int w=0; w<fieldSideLen; w++ ){
            //--- field.mcpos ---
            field.mcpos.set_by_mpos(mcpos.get_mpos().x + w*MapField::entSideLen,
                                    mcpos.get_mpos().y + h*MapField::entSideLen);
            //--- field.nodePPos ---
            lPPos.set(  uDistribution(randEngine), 
                        uDistribution(randEngine) );
            field.nodePPos = field.mcpos.get_ppos() + lPPos;
            //--- field.lNodePPosOff ---
            field.lNodePPosOff = field.nodePPos - this->mcpos.get_ppos();
            //--- field.isLand      暂空置
            //--- field.ecoSysType  暂空置
            fields.push_back( field ); //- copy
        }
    }
}



/* ===========================================================
 *                get_mapFieldPtr_by_mpos
 * -----------------------------------------------------------
 * 传入本chunk中的任意一个 mpos，获得其对应的 MapField 的 访问权 
 * param: _mpos -- 这个 chunk 中的任意 mapent.mpos 
 */
MapField *ChunkFieldSet::get_mapFieldPtr_by_mpos( const IntVec2 &_mpos ){

    IntVec2  lMPosOff = _mpos - mcpos.get_mpos(); 
    IntVec2  fieldWH_ = floorDiv( lMPosOff, (float)fieldSideLen );
    size_t idx = fieldWH_.y*fieldSideLen + fieldWH_.x;
        assert( (idx>=0) && (idx<fields.size()) ); //- tmp
    return (MapField*)&(fields.at(idx));
}



