/*
 * ================== esrc_chunkMemState.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.10
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_chunkMemState.h"
//-------------------- CPP --------------------//
#include <unordered_map>
#include <memory>
#include <deque>
#include <unordered_set>

//-------------------- Engine --------------------//
#include "tprAssert.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

//- only used for esrc_chunkMemState -
extern bool find_from_chunks( chunkKey_t chunkKey_ );

namespace chunk_inn {//------------ namespace: chunk_inn --------------//

    //-- 为了提高 get_chunkMemState() 的速度，而设置的中间层
    // 只要是出现在以下子容器中的 chunkkey，一定也在 chunkMemStates 中
    // 及时跟踪 每个已存 chunk 的 state
    // 很容易出错，需要 手动 debug .....
    std::unordered_map<chunkKey_t,ChunkMemState> chunkMemStates {};

    // 注意，当处于 onCreating 时，chunk实例尚未存在于 chunks 中
    std::unordered_set<chunkKey_t>     chunkKeys_onCreating {};  //- OnCreating
    std::unordered_set<chunkKey_t>     chunkKeys_active {};      //- Active
    std::deque<chunkKey_t>             chunkKeys_waitForRelease {}; //- WaitForRelease
    std::unordered_set<chunkKey_t>     chunkKeys_onReleasing {};    //- OnReleasing

}//---------------- namespace: chunk_inn end --------------//

void chunkMemState_debug( chunkKey_t key_, const std::string &str_ ){
    auto state = get_chunkMemState( key_ );
    cout << "" << str_ << ": ";
    switch (state){
        case ChunkMemState::NotExist: cout << " NotExist" << endl; break;
        case ChunkMemState::OnCreating: cout << " OnCreating" << endl; break;
        case ChunkMemState::Active: cout << " Active" << endl; break;
        case ChunkMemState::WaitForRelease: cout << " WaitForRelease" << endl; break;
        case ChunkMemState::OnReleasing: cout << " OnReleasing" << endl; break;
        default: tprAssert(0); break;
    }
}

//- only used by esrc_chunk -
void chunkStates_debug(){
    cout << "\nchunkKeys_onCreating.size() = " << chunk_inn::chunkKeys_onCreating.size()
        << "\nchunkKeys_active.size() = " << chunk_inn::chunkKeys_active.size()
        << "\nchunks_waitForRelease.size() = " << chunk_inn::chunkKeys_waitForRelease.size()
        << "\nchunks_onReleasing.size() = " << chunk_inn::chunkKeys_onReleasing.size()
        << endl;
}

//- only used by esrc_chunk -
const std::unordered_set<chunkKey_t> &get_chunkKeys_active(){
    return chunk_inn::chunkKeys_active;
}


void insert_2_chunkKeys_onCreating( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::NotExist ); // MUST
    chunk_inn::chunkKeys_onCreating.insert(chunkKey_);
    chunk_inn::chunkMemStates.insert({ chunkKey_, ChunkMemState::OnCreating });
}


void move_chunkKey_from_onCreating_2_active( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnCreating ); // MUST
    size_t eraseNum = chunk_inn::chunkKeys_onCreating.erase(chunkKey_);
    tprAssert( eraseNum == 1);
    chunk_inn::chunkKeys_active.insert(chunkKey_);
    chunk_inn::chunkMemStates.at(chunkKey_) = ChunkMemState::Active;
}


void move_chunkKey_from_active_2_waitForRelease( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::Active ); // MUST
    size_t eraseNum = chunk_inn::chunkKeys_active.erase(chunkKey_);
    tprAssert( eraseNum == 1);
    chunk_inn::chunkKeys_waitForRelease.push_back(chunkKey_);
    chunk_inn::chunkMemStates.at(chunkKey_) = ChunkMemState::WaitForRelease;
}

std::pair<bool,chunkKey_t> pop_front_from_WaitForRelease_and_move_2_onReleasing(){
        
    if( chunk_inn::chunkKeys_waitForRelease.empty() ){
        return std::pair<bool,chunkKey_t>{ false, 0 };
    }
    chunkKey_t key = chunk_inn::chunkKeys_waitForRelease.front();
    chunk_inn::chunkKeys_waitForRelease.pop_front();
        tprAssert( chunk_inn::chunkKeys_onReleasing.find(key) == chunk_inn::chunkKeys_onReleasing.end() );
    chunk_inn::chunkKeys_onReleasing.insert(key);
    chunk_inn::chunkMemStates.at(key) = ChunkMemState::OnReleasing;
    return std::pair<bool,chunkKey_t>{ true, key };
}

//- only used by esrc_chunk -
void erase_chunkKey_from_onReleasing( chunkKey_t chunkKey_ ){
        tprAssert( get_chunkMemState(chunkKey_) == ChunkMemState::OnReleasing );
    size_t eraseNum {};
    eraseNum = chunk_inn::chunkKeys_onReleasing.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
    eraseNum = chunk_inn::chunkMemStates.erase(chunkKey_);
    tprAssert( eraseNum == 1 );
}


/* ===========================================================
 *                  get_chunkMemState
 * -----------------------------------------------------------
 * 高速版
 */
ChunkMemState get_chunkMemState( chunkKey_t chunkKey_ ){    
    if( chunk_inn::chunkMemStates.find(chunkKey_) != chunk_inn::chunkMemStates.end() ){
        return chunk_inn::chunkMemStates.at(chunkKey_);
    }else{
        return ChunkMemState::NotExist;
    }
}





}//---------------------- namespace: esrc -------------------------//

