/*
 * ======================= Section.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#include "Section.h"


//-------------------- C --------------------//
#include <cassert>


//-------------------- Engine --------------------//
#include "random.h"
#include "EcoSysInMap.h"
#include "srcs_engine.h"
#include "Quad.h"


#include "debug.h"

namespace{//-------- namespace: --------------//

    
    //- 随机数引擎实例，暂先用于 本模块的所有 随机数生成
    std::default_random_engine  randEngine; 
                                    
    //-- 用于生成 chunk nodePPos [0,319]  ---
    std::uniform_int_distribution<int> uDistribution_chunkNodePPos(0,
                                        (ENTS_PER_CHUNK*PIXES_PER_MAPENT)-1); 
                                        
    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },
        IntVec2{ 1, 0 },
        IntVec2{ 0, 1 },
        IntVec2{ 1, 1 }
    };

}//------------- namespace: end --------------//



/* ===========================================================
 *                     init
 * -----------------------------------------------------------
 * -- 
 */
void Section::init(){
    
    randEngine.seed( get_new_seed() ); //- tmp

    init_nearbySectionKeys();
    init_quadSectionKeys();
    init_chunkEcoSysInMapKeys();
    init_chunkNodePPoses();
}



/* ===========================================================
 *               init_nearbySectionKeys
 * -----------------------------------------------------------
 * -- 填充 nearbySectionKeys
 *    共9个元素，与 NineBox 次序一致
 */
void Section::init_nearbySectionKeys(){
    IntVec2        mpos = mcpos.get_mpos();
    IntVec2        tmpMPos;
    sectionKey_t   tmpKey;
    //-- 遍历九宫格 --
    nearbySectionKeys.clear();
    for( int h=-1; h<=1; h++  ){
        for( int w=-1; w<=1; w++ ){
            //-- 若为 自己 section --
            if( (h==0) && (w==0) ){
                nearbySectionKeys.push_back( this->sectionKey );//- copy
                continue;
            }
            tmpMPos.x = mpos.x + w*ENTS_PER_SECTION;
            tmpMPos.y = mpos.y + h*ENTS_PER_SECTION;
            tmpKey = sectionMPos_2_key( tmpMPos );
            //---
            nearbySectionKeys.push_back( tmpKey );//- copy
        }
    }
    assert( nearbySectionKeys.size() == 9 );
}



/* ===========================================================
 *               init_quadSectionKeys
 * -----------------------------------------------------------
 * -- 填充 quadSectionKeys
 *    共4个元素
 */
void Section::init_quadSectionKeys(){

    IntVec2        mpos = mcpos.get_mpos();
    IntVec2        tmpMPos;
    sectionKey_t   tmpKey;
    quadSectionKeys.clear();
    for( const auto &whOff : quadSectionKeyOffs ){
        tmpMPos.x = mpos.x + whOff.x*ENTS_PER_SECTION;
        tmpMPos.y = mpos.y + whOff.y*ENTS_PER_SECTION;
        tmpKey = sectionMPos_2_key( tmpMPos );
        //---
        quadSectionKeys.push_back( tmpKey );//- copy
    }
    assert( quadSectionKeys.size() == QUAD_NUM );
}


/* ===========================================================
 *               init_chunkNodePPoses
 * -----------------------------------------------------------
 * -- 随机分配 chunk node ppos
 */
void Section::init_chunkNodePPoses(){

    IntVec2  nodePPos;
    IntVec2  lNodePPosOff;
    IntVec2  tmpChunkMPos;
    IntVec2  sectionMPos = this->mcpos.get_mpos();
    chunkNodePPoses.clear();
    for( int h=0; h<CHUNKS_PER_SECTION; h++ ){
        for( int w=0; w<CHUNKS_PER_SECTION; w++ ){

            lNodePPosOff.set(   uDistribution_chunkNodePPos(randEngine),
                                uDistribution_chunkNodePPos(randEngine) );
            tmpChunkMPos.set(   sectionMPos.x + w*ENTS_PER_CHUNK,
                                sectionMPos.y + h*ENTS_PER_CHUNK );
            nodePPos = tmpChunkMPos + lNodePPosOff;
            chunkNodePPoses.push_back( nodePPos ); //- copy
        }
    }
    assert( chunkNodePPoses.size() == CHUNKS_PER_SECTION*CHUNKS_PER_SECTION );
}



/* ===========================================================
 *                bind_ecoSysInMapPtrs
 * -----------------------------------------------------------
 * -- 绑定 4个端点的 ecosys 指针。
 *   调用之前，应确保 目标 ecosysInMap 实例 已经创建了
 */
void Section::bind_ecoSysInMapPtrs(){
    EcoSysInMap  *ecoSysInMapPtr;
    ecoSysInMapPtrs.clear();
    for( const auto &key : quadSectionKeys ){ //- each key val
        ecoSysInMapPtr = esrc::get_ecoSysInMapPtr(key);
        ecoSysInMapPtrs.push_back( ecoSysInMapPtr ); //- copy        
    }
}




