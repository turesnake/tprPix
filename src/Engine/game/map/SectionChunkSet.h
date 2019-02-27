/*
 * =================== SectionChunkSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section chunk集合 
 * ----------------------------
 */
#ifndef _TPR_SECTION_CHUNK_SET_H
#define _TPR_SECTION_CHUNK_SET_H


//-------------------- CPP --------------------//
#include <vector>


//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "EcoSysType.h"
#include "SectionKey.h"
#include "MapCoord.h"
#include "MapChunk.h"


//-- 一个 section 拥有的 64*64 个 chunk 的集合 --
class SectionChunkSet{
public:

    void init( const IntVec2 &_mpos );

    MapChunk *get_mapChunkPtr_by_mpos( const IntVec2 &_mpos );

    //====== vals =======//
    std::vector<MapChunk>  chunks {}; //- 64*64 个

    //-- 下面两个数据 和 Section 彻底重复了... --
    //  未来考虑 整合之 ...
    MapCoord      mcpos      {}; //- section.mcpos
    SectionKey    sectionKey {}; //- 存储了相邻8个section 的key数据
                        
    
    //====== static =======//
    static  int   chunkSideLen; //- 64*64 个 chunk (只记录边长)
};
//====== static =======//
inline int SectionChunkSet::chunkSideLen { SECTION_SIDE_ENTS/MapChunk::entSideLen }; //- 64


#endif 

