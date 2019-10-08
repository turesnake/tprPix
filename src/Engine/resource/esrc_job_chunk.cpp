/*
 * ==================== esrc_Job_Chunk.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "esrc_job_chunk.h"

//-------------------- CPP --------------------//
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <deque>
#include <memory>

//-------------------- Engine --------------------//
#include "config.h"
#include "tprAssert.h"

#include "esrc_state.h"



namespace esrc {//------------------ namespace: esrc -------------------------//


namespace chunkD_inn {//------------ namespace: chunkD_inn --------------//

    //-- cross threads --
    std::unordered_map<chunkKey_t, std::unique_ptr<Job_Chunk>> job_chunks {};
    std::shared_mutex  sharedMutex;

    //-- 已经被 job线程 制作完毕的 job_chunk, 将向此表 压入一个 元素 --
    std::deque<chunkKey_t> job_chunkFlags {};
    std::mutex  job_chunkFlagsMutex;


}//---------------- namespace: chunkD_inn end --------------//


void init_job_chunks(){
    chunkD_inn::job_chunks.reserve(100);
    esrc::insertState("job_chunk");
}

/* ===========================================================
 *              atom_insert_new_job_chunk      [-WRITE-]
 * -----------------------------------------------------------
 * 通常由 job线程 调用
 */
Job_Chunk &atom_insert_new_job_chunk( chunkKey_t chunkKey_, IntVec2 chunkMPos_ ){
    auto job_chunkUPtr = std::make_unique<Job_Chunk>(chunkKey_, chunkMPos_);
    //--- atom ---//
    {
        std::unique_lock<std::shared_mutex> ul( chunkD_inn::sharedMutex ); //- write
        auto outPair = chunkD_inn::job_chunks.insert({ chunkKey_, std::move(job_chunkUPtr) });
        tprAssert( outPair.second ); //- MUST NOT EXIST
        return *(outPair.first->second);
    }
}


/* ===========================================================
 *          atom_erase_from_job_chunks        [-WRITE-]
 * -----------------------------------------------------------
 * 通常由 主线程 调用
 */
void atom_erase_from_job_chunks( chunkKey_t chunkKey_ ){
    {//--- atom ---//
        std::unique_lock<std::shared_mutex> ul( chunkD_inn::sharedMutex ); //- write
        size_t eraseNum = chunkD_inn::job_chunks.erase(chunkKey_);
        tprAssert( eraseNum == 1 );
    }
}

/* ===========================================================
 *              atom_getnc_job_chunkRef      [-READ-]
 * -----------------------------------------------------------
 * 只能由 主线程 调用
 */
Job_Chunk &atom_getnc_job_chunkRef( chunkKey_t chunkKey_ ){
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( chunkD_inn::sharedMutex ); //- read
        tprAssert( chunkD_inn::job_chunks.find(chunkKey_) != chunkD_inn::job_chunks.end() ); //- MUST EXIST
        return *(chunkD_inn::job_chunks.at(chunkKey_));
    }
}

/* ===========================================================
 *              atom_is_job_chunkFlags_empty
 * -----------------------------------------------------------
 * -- 查看 状态表 是否为空
 */
bool atom_is_job_chunkFlags_empty(){
    bool ret {true};
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkD_inn::job_chunkFlagsMutex );
        ret = chunkD_inn::job_chunkFlags.empty();
    }
    return ret;
}

/* ===========================================================
 *              atom_push_back_2_job_chunkFlags
 * -----------------------------------------------------------
 * -- 通常由 job线程 调用
 */
void atom_push_back_2_job_chunkFlags( chunkKey_t chunkKey_ ){
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkD_inn::job_chunkFlagsMutex );
        chunkD_inn::job_chunkFlags.push_back( chunkKey_ );
    }
}

/* ===========================================================
 *            atom_pop_from_job_chunkFlags
 * -----------------------------------------------------------
 * -- 通常由 主线程 调用
 */
chunkKey_t atom_pop_from_job_chunkFlags(){
    chunkKey_t key {};
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( chunkD_inn::job_chunkFlagsMutex );
        key = chunkD_inn::job_chunkFlags.front();
        chunkD_inn::job_chunkFlags.pop_front();
    }
    return key;
}

}//---------------------- namespace: esrc -------------------------//

