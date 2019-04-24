/*
 * ==================== chunkDatas_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <unordered_map>
#include <mutex>
#include <set>

//-------------------- Engine --------------------//
#include "esrc_chunkData.h"
#include "config.h"

//#include "debug.h"



namespace esrc{ //------------------ namespace: esrc -------------------------//


namespace{//------------ namespace --------------//

    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, ChunkData> chunkDatas {};

    std::mutex  chunkDatasMutex;

    //-- 已经被 job线程 制作完毕的 chunkData, 将向此表 压入一个 元素 --
    std::set<chunkKey_t> chunkDataFlags {};

    std::mutex  chunkDataFlagsMutex;


}//---------------- namespace end --------------//


/* ===========================================================
 *              atom_insert_new_chunkData
 * -----------------------------------------------------------
 * -- 返回一个 全空实例 的指针
 * 通常由 job线程 调用
 */
ChunkData *atom_insert_new_chunkData( chunkKey_t _chunkKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    ChunkData  chunkData {};
    ChunkData *chunkDataPtr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkDatasMutex );
            assert( esrc::chunkDatas.find(_chunkKey) == esrc::chunkDatas.end() );
        esrc::chunkDatas.insert({ _chunkKey, chunkData }); //- copy
        chunkDataPtr = &(esrc::chunkDatas.at(_chunkKey));
    }
    return chunkDataPtr;
}


/* ===========================================================
 *              atom_get_chunkDataPtr
 * -----------------------------------------------------------
 * 通常由 主线程 调用
 */
ChunkData *atom_get_chunkDataPtr( chunkKey_t _chunkKey ){
    ChunkData *ptr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkDatasMutex );
            assert( esrc::chunkDatas.find(_chunkKey) != esrc::chunkDatas.end() );
        ptr = &(esrc::chunkDatas.at(_chunkKey));
    }
    return ptr;
}


/* ===========================================================
 *              atom_insert_new_chunkData
 * -----------------------------------------------------------
 * 通常由 主线程 调用
 */
void atom_erase_from_chunkDatas( chunkKey_t _chunkKey ){
    //--- atom ---//
    std::lock_guard<std::mutex> lg( chunkDatasMutex );
    assert( esrc::chunkDatas.erase(_chunkKey) == 1 );
}




/* ===========================================================
 *              atom_find_is_chunkData_Done
 * -----------------------------------------------------------
 * -- 查看 状态表，目标 chunkData 是否已经 计算完毕
 */
bool atom_find_is_chunkData_Done( chunkKey_t _chunkKey ){
    bool ret;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkDataFlagsMutex );
        ret = esrc::chunkDataFlags.find(_chunkKey) != esrc::chunkDataFlags.end();
    }
    return ret;
}


/* ===========================================================
 *              atom_insert_2_chunkDataFlags
 * -----------------------------------------------------------
 * -- 通常由 job线程 调用
 */
void atom_insert_2_chunkDataFlags( chunkKey_t _chunkKey ){
    //--- atom ---//
    std::lock_guard<std::mutex> lg( chunkDataFlagsMutex );
        assert( esrc::chunkDataFlags.find(_chunkKey) != esrc::chunkDataFlags.end() );
    esrc::chunkDataFlags.insert(_chunkKey);
}

/* ===========================================================
 *              atom_erase_from_chunkDataFlags
 * -----------------------------------------------------------
 * -- 通常由 主线程 调用
 */
void atom_erase_from_chunkDataFlags( chunkKey_t _chunkKey ){
    //--- atom ---//
    std::lock_guard<std::mutex> lg( chunkDataFlagsMutex );
    assert( esrc::chunkDataFlags.erase(_chunkKey) == 1 );
}





}//---------------------- namespace: esrc -------------------------//

