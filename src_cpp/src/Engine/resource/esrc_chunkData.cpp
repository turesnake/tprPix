/*
 * ==================== esrc_chunkData.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- CPP --------------------//
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <deque>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_chunkData.h"
#include "config.h"



namespace esrc {//------------------ namespace: esrc -------------------------//


namespace chunkD_inn {//------------ namespace: chunkD_inn --------------//

    //-- chunks 不跨线程，仅被 主线程访问 --
    std::unordered_map<chunkKey_t, ChunkData> chunkDatas {};
    std::shared_mutex  sharedMutex;

    //-- 已经被 job线程 制作完毕的 chunkData, 将向此表 压入一个 元素 --
    std::deque<chunkKey_t> chunkDataFlags {};
    std::mutex  chunkDataFlagsMutex;

    //===== funcs =====//
    bool is_find_in_chunkDatas_( chunkKey_t key_ ){
        return (chunkD_inn::chunkDatas.find(key_) != chunkD_inn::chunkDatas.end());
    }


}//---------------- namespace: chunkD_inn end --------------//


/* ===========================================================
 *              atom_insert_new_chunkData      [-WRITE-]
 * -----------------------------------------------------------
 * -- 返回一个 全空实例 的指针
 * 通常由 job线程 调用
 *       
 *           这样实现 很不安全，在未来需要修改
 */
ChunkData *atom_insert_new_chunkData( chunkKey_t chunkKey_ ){

    // ***| INSERT FIRST, INIT LATER  |***
    ChunkData  chunkData {};
    ChunkData *chunkDataPtr {};
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( chunkD_inn::sharedMutex ); //- write
            tprAssert( chunkD_inn::is_find_in_chunkDatas_(chunkKey_) == false ); //- MUST NOT EXIST
        chunkD_inn::chunkDatas.insert({ chunkKey_, chunkData }); //- copy
        chunkDataPtr = &(chunkD_inn::chunkDatas.at(chunkKey_));
    }
    return chunkDataPtr;
}


/* ===========================================================
 *          atom_erase_from_chunkDatas        [-WRITE-]
 * -----------------------------------------------------------
 * 通常由 主线程 调用
 */
void atom_erase_from_chunkDatas( chunkKey_t chunkKey_ ){
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( chunkD_inn::sharedMutex ); //- write
        tprAssert( chunkD_inn::chunkDatas.erase(chunkKey_) == 1 );
    }
}



/* ===========================================================
 *              atom_get_chunkData_mapEntAltis      [-READ-]
 * -----------------------------------------------------------
 * 只能由 主线程 调用
 */
const ChunkData *atom_get_chunkDataPtr( chunkKey_t chunkKey_ ){
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( chunkD_inn::sharedMutex ); //- read
            tprAssert( chunkD_inn::is_find_in_chunkDatas_(chunkKey_) ); //- MUST EXIST
        return &(chunkD_inn::chunkDatas.at(chunkKey_));
    }
}



/* ===========================================================
 *              atom_is_chunkDataFlags_empty
 * -----------------------------------------------------------
 * -- 查看 状态表 是否为空
 */
bool atom_is_chunkDataFlags_empty(){
    bool ret {true};
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkD_inn::chunkDataFlagsMutex );
        ret = chunkD_inn::chunkDataFlags.empty();
    }
    return ret;
}


/* ===========================================================
 *              atom_push_back_2_chunkDataFlags
 * -----------------------------------------------------------
 * -- 通常由 job线程 调用
 */
void atom_push_back_2_chunkDataFlags( chunkKey_t chunkKey_ ){
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkD_inn::chunkDataFlagsMutex );
        chunkD_inn::chunkDataFlags.push_back( chunkKey_ );
    }
}

/* ===========================================================
 *            atom_pop_from_chunkDataFlags
 * -----------------------------------------------------------
 * -- 通常由 主线程 调用
 */
chunkKey_t atom_pop_from_chunkDataFlags(){
    chunkKey_t key {};
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkD_inn::chunkDataFlagsMutex );
        key = chunkD_inn::chunkDataFlags.front();
        chunkD_inn::chunkDataFlags.pop_front();
    }
    return key;
}





}//---------------------- namespace: esrc -------------------------//

