/*
 * =================== ChunkFieldSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   fields in chunk 
 * ----------------------------
 */
#ifndef _TPR_CHUNK_FIELD_SET_H
#define _TPR_CHUNK_FIELD_SET_H


//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "config.h"
#include "chunkKey.h"
#include "MapCoord.h"
#include "MapField.h"
#include "fieldKey.h"


//-- 一个 chunk 拥有的 16*16 个 Field 的集合 --
class ChunkFieldSet{
public:

    void init_firstOrderData( const IntVec2 &_anyMPos );

    void assign_fields_2_chunk();

    MapField *get_fieldPtr_by_mpos( const IntVec2 &_anyMPos );
    MapField *get_fieldPtr_by_key( fieldKey_t _key );


    //====== vals =======//
    std::unordered_map<fieldKey_t,MapField> fields {}; //- 16*16 个
    
    //----- 一阶数据 / first order data ------//
    //-- 下面两个数据 和 Chunk 彻底重复了... --
    //  未来考虑 整合之 ...
    MapCoord      mcpos    {}; //- chunk.mcpos
    chunkKey_t    chunkKey {}; //- 存储了相邻8个chunk 的key数据


    //----- 二阶数据 / second order data ------//
    // 暂无数据...


    //====== vals =======//
    bool  is_firstOrderData_init {false};
    bool  is_secondOrderData_init {false};
                        
    
private:

    void reset_nearby_chunk_datas();

};


std::vector<ChunkFieldSet*> &get_nearby_chunkFieldSetPtrs( const IntVec2 &_anyMPos );

MapField *get_fieldPtr_by_key( fieldKey_t _key );


#endif 

