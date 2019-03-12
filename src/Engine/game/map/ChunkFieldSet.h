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

    inline void set_by_anyMPos( const IntVec2 &_anyMPos ){
        this->chunkKey = anyMPos_2_chunkKey( _anyMPos );
        this->mcpos.set_by_mpos( chunkKey_2_mpos( this->chunkKey ) );
    }
    inline void set_by_chunkKey( chunkKey_t _key ){
        this->chunkKey = _key;
        this->mcpos.set_by_mpos( chunkKey_2_mpos( this->chunkKey ) );
    }


    void init_fields();
    void assign_fields_2_chunk();
    void assign_alti_in_field();

    MapField *get_fieldPtr_by_mpos( const IntVec2 &_anyMPos );
    MapField *get_fieldPtr_by_key( fieldKey_t _key );

    //====== vals =======//
    std::unordered_map<fieldKey_t,MapField> fields {}; //- 16*16 个
    
    //-- 下面两个数据 和 Chunk 彻底重复了... --
    //  未来考虑 整合之 ...
    MapCoord      mcpos    {}; //- chunk.mcpos
    chunkKey_t    chunkKey {}; //- 存储了相邻8个chunk 的key数据

    //====== flags =======//
    bool  is_fields_set                 {false};
    bool  is_assign_fields_2_chunk_done {false};
    bool  is_assign_alti_in_field_done  {false};
                        
private:
    void reset_nearby_chunk_datas();

};


MapField *get_fieldPtr_by_key( fieldKey_t _key );




#endif 

