/*
 * ============== SectionChunkSetInBuild.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section chunk集合 
 * ----------------------------
 */
#ifndef _TPR_SECTION_CHUNK_SET_IN_BUILD_H
#define _TPR_SECTION_CHUNK_SET_IN_BUILD_H


//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "MapCoord.h"
#include "MapChunk.h"
#include "MapChunkInBuild.h"
#include "SectionChunkSet.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


//-- 一个 section 拥有的 64*64 个 chunk 的集合 --
//  本 class 仅有一个实例，存储在 sectionBuild.h 中
//  每一个正在被创建的 section，都临时使用这个 实例
class SectionChunkSetInBuild{
public:

    void init( SectionChunkSet *_sectionChunkSetPtr );

    

    //====== vals =======//
    std::vector<MapChunkInBuild>  chunksInBuild {}; //- 64*64 个
                //- 建造 section 需要的 临时性 chunk数据 
                //  本clss 存在的根本意义。

    SectionChunkSet  *sectionChunkSetPtr {nullptr};
                //- 指向 当前正在build 的 section 的 数据
    
};


}//----------------- namespace: sectionBuild: end -------------------//
#endif 

