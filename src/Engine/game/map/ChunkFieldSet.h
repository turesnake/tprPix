/*
 * =================== ChunkFieldSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk Field集合 
 * ----------------------------
 */
#ifndef _TPR_CHUNK_FIELD_SET_H
#define _TPR_CHUNK_FIELD_SET_H


//-------------------- CPP --------------------//
#include <vector>


//-------------------- Engine --------------------//
#include "config.h"
#include "ChunkKey.h"
#include "MapCoord.h"
#include "MapField.h"


//-- 一个 chunk 拥有的 64*64 个 Field 的集合 --
class ChunkFieldSet{
public:

    void init( const IntVec2 &_mpos );

    MapField *get_mapFieldPtr_by_mpos( const IntVec2 &_mpos );

    //====== vals =======//
    std::vector<MapField>  fields {}; //- 64*64 个

    //-- 下面两个数据 和 Chunk 彻底重复了... --
    //  未来考虑 整合之 ...
    MapCoord      mcpos      {}; //- chunk.mcpos
    ChunkKey    chunkKey {}; //- 存储了相邻8个chunk 的key数据
                        
    
    //====== static =======//
    static  int   fieldSideLen; //- 64*64 个 field (只记录边长)
};
//====== static =======//
inline int ChunkFieldSet::fieldSideLen { ENTS_PER_CHUNK/MapField::entSideLen }; //- 64


#endif 

