/*
 * ================ ChunkCreateReleaseZone.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ChunkCreateReleaseZone.h"

//-------------------- Engine --------------------//
#include "chunkRelease.h"
#include "esrc_chunk.h"
#include "esrc_player.h"

#include "tprDebug.h"

/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void ChunkCreateReleaseZone::init( const IntVec2 &playerMPos_ ){

    //-------------------------//
    // 装填 old-chunkkeys 容器
    //-------------------------//
    this->is_currentOld_in_A = true;

    IntVec2     playerChunkMPos = anyMPos_2_chunkMPos( playerMPos_ );
    IntVec2     tmpChunkMPos {};
    chunkKey_t  chunkKey     {};

    this->createZoneUPtr->set_centerChunkMPos( playerChunkMPos );
    this->releaseZoneUPtr->set_centerChunkMPos( playerChunkMPos );

    const auto &chunkOffMPoses = this->releaseZoneUPtr->get_chunkOffMPoses();
    for( const auto &iOffMPos : chunkOffMPoses ){
        tmpChunkMPos = playerChunkMPos + iOffMPos;
        chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
        this->releaseChunkKeys_A.insert(chunkKey);
    }
}

/* ===========================================================
 *                 refresh_and_release
 * -----------------------------------------------------------
 * main
 */
void ChunkCreateReleaseZone::refresh_and_release( const IntVec2 &playerMPos_ ){

    //------------------------//
    //   refresh  createZone
    //------------------------//
    IntVec2 playerChunkMPos = anyMPos_2_chunkMPos( playerMPos_ );
    this->createZoneUPtr->set_centerChunkMPos( playerChunkMPos );

    //------------------------//
    //   check  createZone
    //------------------------//
    IntVec2 offMPos = playerChunkMPos - this->releaseZoneUPtr->get_centerChunkMPos();

    bool is_X_overStep {};
    bool is_Y_overStep {};
    (std::abs(offMPos.x)>this->LimitOff) ? is_X_overStep=true : is_X_overStep=false;
    (std::abs(offMPos.y)>this->LimitOff) ? is_Y_overStep=true : is_Y_overStep=false;

    if( (!is_X_overStep) && (!is_Y_overStep) ){
        return;
    }

    //------------------------//
    //  Now, the releaseZone MUST be moved;
    //  calc new releaseZone centerChunkMPos
    //------------------------//
    IntVec2 newMPos = this->releaseZoneUPtr->get_centerChunkMPos();
    //-- x --
    if( is_X_overStep ){
        (offMPos.x >= 0) ? newMPos.x-=LimitOff : newMPos.x+=LimitOff;
    }
    //-- y --
    if( is_Y_overStep ){
        (offMPos.y >= 0) ? newMPos.y-=LimitOff : newMPos.y+=LimitOff;
    }
    this->releaseZoneUPtr->set_centerChunkMPos( newMPos );


    //------------------------//
    //    old/new chunkKeys
    //    只有在 确定 释放圈 发生位移时，才执行此部分 
    //------------------------//
    std::unordered_set<chunkKey_t> *oldChunkKeysPtr;
    std::unordered_set<chunkKey_t> *newChunkKeysPtr;
                //-- 此处存在隐患，指向 set 的指针 是否会在添加元素时失效 ？？？？？
    if( this->is_currentOld_in_A ){
        oldChunkKeysPtr = &this->releaseChunkKeys_A;
        newChunkKeysPtr = &this->releaseChunkKeys_B;
    }else{
        oldChunkKeysPtr = &this->releaseChunkKeys_B;
        newChunkKeysPtr = &this->releaseChunkKeys_A;
    }
    //-- switch A/B --
    (this->is_currentOld_in_A) ?
            this->is_currentOld_in_A = false :
            this->is_currentOld_in_A = true;

    newChunkKeysPtr->clear();

    IntVec2     tmpChunkMPos {};
    chunkKey_t  chunkKey     {};
    const auto &chunkOffMPoses = this->releaseZoneUPtr->get_chunkOffMPoses();
    for( const auto &iOffMPos : chunkOffMPoses ){
        tmpChunkMPos = playerChunkMPos + iOffMPos;
        chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
        newChunkKeysPtr->insert(chunkKey);
    }
    
            tprAssert( newChunkKeysPtr->size() == this->releaseZoneUPtr->get_chunks() ); //- tmp
            tprAssert( oldChunkKeysPtr->size() == this->releaseZoneUPtr->get_chunks() ); //- tmp

    
    //------------------------//
    //  遍历 old-set 元素，如果某元素不在 new-set 中
    //  说明这就是我们要 释放的 key
    //------------------------//
    for( auto &key : *oldChunkKeysPtr ){ //- each chunkKey in old-set
        if( newChunkKeysPtr->find(key) == newChunkKeysPtr->end() ){

            auto chunkState = esrc::get_chunkMemState(key);
            switch (chunkState){
                case ChunkMemState::NotExist:
                    // do nothing
                    break;
                case ChunkMemState::OnCreating:
                    // 想要删除的居然在新建 ？？？ 直接报错
                    cout << "OnCreating" << endl;
                    tprAssert(0);
                    break;
                case ChunkMemState::Active:
                    chunkRelease::release_chunk( key );
                    break;
                case ChunkMemState::WaitForRelease:
                    // already in release que
                    break;
                case ChunkMemState::OnReleasing:
                    // already in release que
                    // but seems too slow...
                    cout << "OnReleasing" << endl;
                    tprAssert(0);
                    break;
                default:
                    tprAssert(0);
                    break;
            }   
        }
    }//- each chunkKey in old-set
    
}





