/*
 * ================== esrc_chunkMemState.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.10
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_chunkMemState.h"
//-------------------- CPP --------------------//
#include <deque>

//-------------------- Engine --------------------//
#include "esrc_state.h"



namespace esrc {//------------------ namespace: esrc -------------------------//

//- only used for esrc_chunkMemState -
//extern bool find_from_chunks( chunkKey_t chunkKey_ );

namespace chunkMS_inn {//------------ namespace: chunkMS_inn --------------//

    //-- 为了提高 get_chunkMemState() 的速度，而设置的中间层
    // 只要是出现在以下子容器中的 chunkkey，一定也在 chunkMemStates 中
    // 及时跟踪 每个已存 chunk 的 state
    // 很容易出错，需要 手动 debug .....
    std::unordered_map<chunkKey_t,ChunkMemState> chunkMemStates {};

    // 注意，当处于 onCreating 时，chunk实例尚未存在于 chunks 中
    std::unordered_set<chunkKey_t>     chunkKeys_waitForCreate {};  //- WaitForCreate
    std::unordered_set<chunkKey_t>     chunkKeys_onCreating {};  //- OnCreating
    std::unordered_set<chunkKey_t>     chunkKeys_active {};      //- Active
    std::deque<chunkKey_t>             chunkKeys_waitForRelease {}; //- WaitForRelease
    std::unordered_set<chunkKey_t>     chunkKeys_onReleasing {};    //- OnReleasing

    // 还有一个 容器：chunkKeys_waitForDB
    // 如果一个 chunk 为 diry，会先被移入此容器，等待job线程完成 db存储工作后，再被移入 chunkKeys_waitForRelease 容器
    // 如果 非dirty，则被直接移入 chunkKeys_waitForRelease 容器
    // ...

}//---------------- namespace: chunkMS_inn end --------------//


void init_chunkMemStates()noexcept{
    chunkMS_inn::chunkMemStates.reserve(1000);
    //---
    chunkMS_inn::chunkKeys_waitForCreate.reserve(100);
    chunkMS_inn::chunkKeys_onCreating.reserve(100);
    chunkMS_inn::chunkKeys_active.reserve(100);
    chunkMS_inn::chunkKeys_onReleasing.reserve(100);
    //---
    esrc::insertState("chunkMemState");
}

void chunkMemState_debug( chunkKey_t key_, const std::string &str_ ){
    auto state = get_chunkMemState( key_ );
    switch (state){
        case ChunkMemState::NotExist:           tprDebug::console( "{}: NotExist", str_ ); break;
        case ChunkMemState::WaitForCreate:      tprDebug::console( "{}: NotExist", str_ ); break;
        case ChunkMemState::OnCreating:         tprDebug::console( "{}: OnCreating", str_ ); break;
        case ChunkMemState::Active:             tprDebug::console( "{}: Active", str_ ); break;
        case ChunkMemState::WaitForRelease:     tprDebug::console( "{}: WaitForRelease", str_ ); break;
        case ChunkMemState::OnReleasing:        tprDebug::console( "{}: OnReleasing", str_ ); break;
        default: tprAssert(0); break;
    }
}

//- only used by esrc_chunk -
void chunkStates_debug(){
    tprDebug::console(
        "\nchunkKeys_onCreating.size() = {0}" \
        "\nchunkKeys_active.size() = {1}" \
        "\nchunks_waitForRelease.size() = {2}" \
        "\nchunks_onReleasing.size() = {3}",
        chunkMS_inn::chunkKeys_onCreating.size(),
        chunkMS_inn::chunkKeys_active.size(),
        chunkMS_inn::chunkKeys_waitForRelease.size(),
        chunkMS_inn::chunkKeys_onReleasing.size()
    );
}

//- only used by esrc_chunk -
const std::unordered_set<chunkKey_t> &get_chunkKeys_active()noexcept{
    return chunkMS_inn::chunkKeys_active;
}


void insert_chunkKey_2_waitForCreate( chunkKey_t chunkKey_ )noexcept{
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::NotExist ); // MUST
    chunkMS_inn::chunkKeys_waitForCreate.insert( chunkKey_ );
    chunkMS_inn::chunkMemStates.insert({ chunkKey_, ChunkMemState::WaitForCreate });
}



void insert_chunkKey_2_onCreating( chunkKey_t chunkKey_ )noexcept{

                if( get_chunkMemState(chunkKey_) != ChunkMemState::NotExist ){
                    chunkMemState_debug( chunkKey_,  "insert_chunkKey_2_onCreating()" );
                }

        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::NotExist ); // MUST

    chunkMS_inn::chunkKeys_onCreating.insert(chunkKey_);
    chunkMS_inn::chunkMemStates.insert({ chunkKey_, ChunkMemState::OnCreating });
}


const std::unordered_set<chunkKey_t> &get_chunkKeys_waitForCreate()noexcept{
    return chunkMS_inn::chunkKeys_waitForCreate;
}



void move_chunkKey_from_waitForCreate_2_onCreating( chunkKey_t chunkKey_ )noexcept{
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::WaitForCreate ); // MUST
    size_t eraseNum = chunkMS_inn::chunkKeys_waitForCreate.erase(chunkKey_);
    tprAssert( eraseNum == 1);
    chunkMS_inn::chunkKeys_onCreating.insert(chunkKey_);
    chunkMS_inn::chunkMemStates.at(chunkKey_) = ChunkMemState::OnCreating;
}



void move_chunkKey_from_onCreating_2_active( chunkKey_t chunkKey_ )noexcept{
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnCreating ); // MUST
    size_t eraseNum = chunkMS_inn::chunkKeys_onCreating.erase(chunkKey_);
    tprAssert( eraseNum == 1);
    chunkMS_inn::chunkKeys_active.insert(chunkKey_);
    chunkMS_inn::chunkMemStates.at(chunkKey_) = ChunkMemState::Active;
}


void move_chunkKey_from_active_2_waitForRelease( chunkKey_t chunkKey_ )noexcept{
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::Active ); // MUST
    size_t eraseNum = chunkMS_inn::chunkKeys_active.erase(chunkKey_);
    tprAssert( eraseNum == 1);
    chunkMS_inn::chunkKeys_waitForRelease.push_back(chunkKey_);
    chunkMS_inn::chunkMemStates.at(chunkKey_) = ChunkMemState::WaitForRelease;
}


std::optional<chunkKey_t> pop_front_from_WaitForRelease_and_move_2_onReleasing()noexcept{
        
    if( chunkMS_inn::chunkKeys_waitForRelease.empty() ){
        return std::nullopt;
    }
    chunkKey_t key = chunkMS_inn::chunkKeys_waitForRelease.front();
    chunkMS_inn::chunkKeys_waitForRelease.pop_front();
        tprAssert( chunkMS_inn::chunkKeys_onReleasing.find(key) == chunkMS_inn::chunkKeys_onReleasing.end() );
    chunkMS_inn::chunkKeys_onReleasing.insert(key);
    chunkMS_inn::chunkMemStates.at(key) = ChunkMemState::OnReleasing;
    return { key };
}


//- only used by esrc_chunk -
void erase_chunkKey_from_onReleasing( chunkKey_t chunkKey_ )noexcept{
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnReleasing );
    size_t eraseNum {};
    eraseNum = chunkMS_inn::chunkKeys_onReleasing.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
    eraseNum = chunkMS_inn::chunkMemStates.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
}


/* ===========================================================
 *                  get_chunkMemState
 * -----------------------------------------------------------
 */
ChunkMemState get_chunkMemState( chunkKey_t chunkKey_ )noexcept{    
    if( chunkMS_inn::chunkMemStates.find(chunkKey_) != chunkMS_inn::chunkMemStates.end() ){
        return chunkMS_inn::chunkMemStates.at(chunkKey_);
    }else{
        return ChunkMemState::NotExist;
    }
}





}//---------------------- namespace: esrc -------------------------//

