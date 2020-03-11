/*
 * ==================== esrc_Job_Chunk.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_job_chunk.h"

//-------------------- CPP --------------------//
#include <mutex>
#include <shared_mutex>
#include <deque>

//-------------------- Engine --------------------//
#include "esrc_state.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace jchunk_inn {//------------ namespace: jchunk_inn --------------//

    //-- cross threads --
    std::unordered_map<chunkKey_t, std::unique_ptr<Job_Chunk>> job_chunks {};
    std::shared_mutex  sharedMutex;

    //-- 已经被 job线程 制作完毕的 job_chunk, 将向此表 压入一个 元素 --
    std::deque<chunkKey_t> job_chunkFlags {};
    std::mutex  job_chunkFlagsMutex;


}//---------------- namespace: jchunk_inn end --------------//


void init_job_chunks(){
    jchunk_inn::job_chunks.reserve(100);
    esrc::insertState("job_chunk");
}

/* ===========================================================
 *              atom_insert_new_job_chunk      [-WRITE-]
 * -----------------------------------------------------------
 * 通常由 job线程 调用
 */
Job_Chunk &atom_insert_new_job_chunk( chunkKey_t chunkKey_ ){
    auto job_chunkUPtr = std::make_unique<Job_Chunk>(chunkKey_);
    //--- atom ---//
    std::unique_lock<std::shared_mutex> ul( jchunk_inn::sharedMutex ); //- write
    auto [insertIt, insertBool] = jchunk_inn::job_chunks.emplace( chunkKey_, std::move(job_chunkUPtr) );
    tprAssert( insertBool ); //- MUST NOT EXIST
    return *(insertIt->second);
}


/* ===========================================================
 *          atom_erase_from_job_chunks        [-WRITE-]
 * -----------------------------------------------------------
 * 通常由 主线程 调用
 */
void atom_erase_from_job_chunks( chunkKey_t chunkKey_ ){
    //--- atom ---//
    std::unique_lock<std::shared_mutex> ul( jchunk_inn::sharedMutex ); //- write
    size_t eraseNum = jchunk_inn::job_chunks.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
}

/* ===========================================================
 *              atom_getnc_job_chunk_ptr     [-READ-]
 * -----------------------------------------------------------
 * 只能由 主线程 调用
 */
std::optional<Job_Chunk*> atom_getnc_job_chunk_ptr( chunkKey_t chunkKey_ ){
    //--- atom ---//
    std::shared_lock<std::shared_mutex> sl( jchunk_inn::sharedMutex ); //- read
    if( jchunk_inn::job_chunks.find(chunkKey_) == jchunk_inn::job_chunks.end() ){
        return std::nullopt;
    }else{
        return { jchunk_inn::job_chunks.at(chunkKey_).get() };
    }
}



/* ===========================================================
 *              atom_push_back_2_job_chunkFlags
 * -----------------------------------------------------------
 * -- 通常由 job线程 调用
 */
void atom_push_back_2_job_chunkFlags( chunkKey_t chunkKey_ ){
    //--- atom ---//
    std::lock_guard<std::mutex> lg( jchunk_inn::job_chunkFlagsMutex );
    jchunk_inn::job_chunkFlags.push_back( chunkKey_ );
}



/* ===========================================================
 *            atom_pop_from_job_chunkFlags
 * -----------------------------------------------------------
 * -- 通常由 主线程 调用
 */
std::optional<chunkKey_t> atom_pop_from_job_chunkFlags(){
    
    {//--- atom ---//
        std::lock_guard<std::mutex> lg( jchunk_inn::job_chunkFlagsMutex );
        if( jchunk_inn::job_chunkFlags.empty() ){
            return std::nullopt;
        }else{
            chunkKey_t key = jchunk_inn::job_chunkFlags.front();
            jchunk_inn::job_chunkFlags.pop_front();
            return { key };
        }
    }
}

}//---------------------- namespace: esrc -------------------------//

