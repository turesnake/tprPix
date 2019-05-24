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
#include <shared_mutex>
#include <deque>

//-------------------- Engine --------------------//
#include "esrc_chunkData.h"
#include "config.h"


//#include "debug.h"

/*
#include <iostream>
#include <string>
using std::cout;
using std::endl;
*/



namespace esrc{ //------------------ namespace: esrc -------------------------//


namespace{//------------ namespace --------------//

    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, ChunkData> chunkDatas {};
    std::shared_mutex  sharedMutex;

    //-- 已经被 job线程 制作完毕的 chunkData, 将向此表 压入一个 元素 --
    std::deque<chunkKey_t> chunkDataFlags {};
    std::mutex  chunkDataFlagsMutex;

    //===== funcs =====//
    bool is_find_in_chunkDatas_( chunkKey_t _key ){
        return (esrc::chunkDatas.find(_key) != esrc::chunkDatas.end());
    }


}//---------------- namespace end --------------//


/* ===========================================================
 *              atom_insert_new_chunkData      [-WRITE-]
 * -----------------------------------------------------------
 * -- 返回一个 全空实例 的指针
 * 通常由 job线程 调用
 *       
 *           这样实现 很不安全，在未来需要修改
 */
ChunkData *atom_insert_new_chunkData( chunkKey_t _chunkKey ){

    // ***| INSERT FIRST, INIT LATER  |***
    ChunkData  chunkData {};
    ChunkData *chunkDataPtr;
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( sharedMutex ); //- write
            assert( is_find_in_chunkDatas_(_chunkKey) == false ); //- MUST NOT EXIST
        esrc::chunkDatas.insert({ _chunkKey, chunkData }); //- copy
        chunkDataPtr = &(esrc::chunkDatas.at(_chunkKey));
    }
    return chunkDataPtr;
}


/* ===========================================================
 *          atom_erase_from_chunkDatas        [-WRITE-]
 * -----------------------------------------------------------
 * 通常由 主线程 调用
 */
void atom_erase_from_chunkDatas( chunkKey_t _chunkKey ){
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( sharedMutex ); //- write
        assert( esrc::chunkDatas.erase(_chunkKey) == 1 );
    }
}



/* ===========================================================
 *              atom_get_chunkData_mapEntAltis      [-READ-]
 * -----------------------------------------------------------
 * 只能由 主线程 调用
 */
const ChunkData *atom_get_chunkDataPtr( chunkKey_t _chunkKey ){
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( sharedMutex ); //- read
            assert( is_find_in_chunkDatas_(_chunkKey) ); //- MUST EXIST
        return &(esrc::chunkDatas.at(_chunkKey));
    }
}



/* ===========================================================
 *              atom_is_chunkDataFlags_empty
 * -----------------------------------------------------------
 * -- 查看 状态表 是否为空
 */
bool atom_is_chunkDataFlags_empty(){
    bool ret;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkDataFlagsMutex );
        ret = esrc::chunkDataFlags.empty();
    }
    return ret;
}


/* ===========================================================
 *              atom_push_back_2_chunkDataFlags
 * -----------------------------------------------------------
 * -- 通常由 job线程 调用
 */
void atom_push_back_2_chunkDataFlags( chunkKey_t _chunkKey ){
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkDataFlagsMutex );
        esrc::chunkDataFlags.push_back( _chunkKey );
    }
}

/* ===========================================================
 *            atom_pop_from_chunkDataFlags
 * -----------------------------------------------------------
 * -- 通常由 主线程 调用
 */
chunkKey_t atom_pop_from_chunkDataFlags(){
    chunkKey_t key;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkDataFlagsMutex );
        key = esrc::chunkDataFlags.front();
        esrc::chunkDataFlags.pop_front();
    }
    return key;
}





}//---------------------- namespace: esrc -------------------------//

