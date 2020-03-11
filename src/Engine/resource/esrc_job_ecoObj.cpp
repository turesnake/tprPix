/*
 * ==================== esrc_job_ecoObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_job_ecoObj.h"

//-------------------- CPP --------------------//
#include <mutex>
#include <shared_mutex>


//-------------------- Engine --------------------//
#include "esrc_state.h"
#include "esrc_ecoObj.h"

namespace esrc {//------------------ namespace: esrc -------------------------//
namespace jeo_inn {//------------ namespace: jeo_inn --------------//

    //-- cross threads --
    std::unordered_map<sectionKey_t, std::unique_ptr<EcoObj>> job_ecoObjs {};
    std::shared_mutex  sharedMutex;

    //-- 已经被 job线程 制作完毕的 job_ecoObj, 将向此表 压入一个 元素 --
    std::set<sectionKey_t> job_ecoObjFlags {};
    std::mutex  job_ecoObjFlagsMutex;


}//---------------- namespace: jeo_inn end --------------//


// [-WRITE-]
// [* job-threads *]
EcoObj &atom_insert_new_job_ecoObj( sectionKey_t ecoObjKey_ ){
    auto job_ecoObjUPtr = std::make_unique<EcoObj>();
    //--- atom ---//
    {
        std::unique_lock<std::shared_mutex> ul( jeo_inn::sharedMutex ); //- write
        auto [insertIt, insertBool] = jeo_inn::job_ecoObjs.emplace( ecoObjKey_, std::move(job_ecoObjUPtr) );
        tprAssert( insertBool ); //- MUST NOT EXIST
        return *(insertIt->second);
    }
}


// [* job-threads *]
void atom_insert_2_job_ecoObjFlags( sectionKey_t ecoObjKey_ ){
    //--- atom ---//
    std::lock_guard<std::mutex> lg( jeo_inn::job_ecoObjFlagsMutex );
    auto [insertIt, insertBool] = jeo_inn::job_ecoObjFlags.insert( ecoObjKey_ );
    tprAssert( insertBool );
}


// [-WRITE-]
// [* main-thread *]
// 一次性把所有已创建好的 ecoobjUPtr，move 到 esrc 容器
size_t atom_move_all_ecoObjUptrs_from_job_2_esrc(){

    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( jeo_inn::sharedMutex ); //- read
        if( jeo_inn::job_ecoObjFlags.empty() ){
            return 0; // 
        }
    }

    // 由于 本函数只被 main-thread 调用，所以这么分段加锁 是没问题的。

    //--- atom ---//
    std::unique_lock<std::shared_mutex> ul( jeo_inn::sharedMutex ); //- write
    size_t eraseNum {};

    for( const auto &key : jeo_inn::job_ecoObjFlags ){
        tprAssert( jeo_inn::job_ecoObjs.find(key) != jeo_inn::job_ecoObjs.end() ); //- MUST EXIST
        moveIn_ecoObjUPtr_from_job( key,  std::move( jeo_inn::job_ecoObjs.at(key) ) ); // move
        //--
        eraseNum = jeo_inn::job_ecoObjs.erase( key );
        tprAssert( eraseNum == 1 );
        //--
        move_ecoObjKey_from_onCreating_2_active( key ); // memState
    }
    //---
    size_t retNum = jeo_inn::job_ecoObjFlags.size();
    jeo_inn::job_ecoObjFlags.clear();
    return retNum;
}





}//---------------------- namespace: esrc -------------------------//

