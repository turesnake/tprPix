/*
 * ================ ChunkCreateReleaseZone.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ChunkCreateReleaseZone.h"

//-------------------- Engine --------------------//
#include "chunkRelease.h"

#include "esrc_chunk.h"
#include "esrc_player.h"


//namespace ccrz_inn {//------------------ namespace: ccrz_inn -------------------------//
//}//---------------------- namespace: ccrz_inn -------------------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void ChunkCreateReleaseZone::init( IntVec2 playerMPos_ ){

    //-------------------------//
    // 装填 old-chunkkeys 容器
    //-------------------------//
    IntVec2     tmpChunkMPos {};
    chunkKey_t  chunkKey     {};

    this->releaseZoneCenterChunkMCPos = anyMPos_2_chunkMPos(playerMPos_);

    this->is_currentOld_in_A = true;
    for( const auto &iOffMPos : this->releaseZoneOffMPoses ){
        tmpChunkMPos = this->releaseZoneCenterChunkMCPos + iOffMPos;
        chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
        this->releaseChunkKeys_A.insert(chunkKey);
    }
}


/* ===========================================================
 *                 refresh_and_collect_chunks_need_to_be_release
 * -----------------------------------------------------------
 * main
 */
void ChunkCreateReleaseZone::refresh_and_collect_chunks_need_to_be_release( IntVec2 playerMPos_ ){

    //------------------------//
    //   check  createZone
    //------------------------//
    IntVec2 offMPos = anyMPos_2_chunkMPos(playerMPos_) - this->releaseZoneCenterChunkMCPos;

    bool is_X_overStep {};
    bool is_Y_overStep {};
    (std::abs(offMPos.x)>this->LimitOff) ? is_X_overStep=true : is_X_overStep=false;
    (std::abs(offMPos.y)>this->LimitOff) ? is_Y_overStep=true : is_Y_overStep=false;

    if( (!is_X_overStep) && (!is_Y_overStep) ){
        return;
    }

    //------------------------//
    //  Now, the releaseZone MUST be moved;
    //------------------------//
    IntVec2 overflow {0,0}; // 本次 释放圈 需要位移的向量
    if( offMPos.x > this->LimitOff ){
        overflow.x = offMPos.x - this->LimitOff;
    }
    if( offMPos.x < -this->LimitOff ){
        overflow.x = offMPos.x + this->LimitOff;
    }
    if( offMPos.y > this->LimitOff ){
        overflow.y = offMPos.y - this->LimitOff;
    }
    if( offMPos.y < -this->LimitOff ){
        overflow.y = offMPos.y + this->LimitOff;
    }
    this->releaseZoneCenterChunkMCPos += overflow;

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
    for( const auto &iOffMPos : this->releaseZoneOffMPoses ){
        tmpChunkMPos = this->releaseZoneCenterChunkMCPos + iOffMPos;
        chunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
        newChunkKeysPtr->insert(chunkKey);
    }
            tprAssert( newChunkKeysPtr->size() == this->releaseZone.get_chunkNums() ); //- tmp
            tprAssert( oldChunkKeysPtr->size() == this->releaseZone.get_chunkNums() ); //- tmp
    
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
                case ChunkMemState::WaitForCreate:
                case ChunkMemState::OnCreating:
                    // 想要删除的居然在新建(或等待新建) ？？？ 直接报错
                    tprAssert(0);
                    break;
                case ChunkMemState::Active:
                    //chunkRelease::release_a_chunk( key );
                    //-- 符合要求，将此chunk，添加到 释放队列
                    esrc::move_chunkKey_from_active_2_waitForRelease( key );
                    break;
                case ChunkMemState::WaitForRelease:
                    // already in release que
                    break;
                case ChunkMemState::OnReleasing:
                    // already in release que
                    // but seems too slow...
                    tprAssert(0);
                    break;
                default:
                    tprAssert(0);
                    break;
            }   
        }
    }//- each chunkKey in old-set
}


/* ===========================================================
 *               init_createZoneOffMPosesSets
 * -----------------------------------------------------------
 */
void ChunkCreateReleaseZone::init_createZoneOffMPosesSets(){
    
    IntVec2 left_bottom { -1, -1 };
    IntVec2 left_mid    { -1,  0 };
    IntVec2 left_top    { -1,  1 };
    //--
    IntVec2 mid_bottom {  0, -1 };
    IntVec2 mid_mid    {  0,  0 };
    IntVec2 mid_top    {  0,  1 };
    //--
    IntVec2 right_bottom {  1, -1 };
    IntVec2 right_mid    {  1,  0 };
    IntVec2 right_top    {  1,  1 };
    //--
    this->createZoneOffMPosesSets.insert({ NineDirection::LeftBottom,
        std::vector<IntVec2>{ mid_mid, left_bottom, left_mid, mid_bottom, left_top, right_bottom, mid_top, right_mid }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::LeftTop,
        std::vector<IntVec2>{ mid_mid, left_top, mid_top, left_mid, left_bottom, right_top, mid_bottom, right_mid }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::RightTop,
        std::vector<IntVec2>{ mid_mid, right_top, mid_top, right_mid, left_top, right_bottom, left_mid, mid_bottom }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::RightBottom,
        std::vector<IntVec2>{ mid_mid, right_bottom, mid_bottom, right_mid, right_top, left_bottom, left_mid, mid_top }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::Left,
        std::vector<IntVec2>{ mid_mid, left_mid, left_bottom, left_top, mid_bottom, mid_top, right_bottom, right_top }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::Bottom,
        std::vector<IntVec2>{ mid_mid, mid_bottom, left_bottom, right_bottom, left_mid, right_mid, left_top, right_top }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::Right,
        std::vector<IntVec2>{ mid_mid, right_mid, right_bottom, right_top, mid_bottom, mid_top, left_bottom, left_top }
    });
    this->createZoneOffMPosesSets.insert({ NineDirection::Top,
        std::vector<IntVec2>{ mid_mid, mid_top, left_top, right_top, left_mid, right_mid, left_bottom, right_bottom }
    });
    for( auto &[iDir, iVector] : this->createZoneOffMPosesSets ){
        for( auto &mpos : iVector ){
            mpos *= ENTS_PER_CHUNK<>;
        }
    }
}


/* ===========================================================
 *             init_releaseZoneOffMPoses
 * -----------------------------------------------------------
 */
void ChunkCreateReleaseZone::init_releaseZoneOffMPoses(){

    int offLen = this->releaseZone.get_offLen();
    for( int h=-offLen; h<=offLen; h++ ){
        for( int w=-offLen; w<=offLen; w++ ){
            this->releaseZoneOffMPoses.emplace_back( w*ENTS_PER_CHUNK<>, h*ENTS_PER_CHUNK<> );
        }
    }
}


