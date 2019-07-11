/*
 * ================= ChunkCreateReleaseZone.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHUNK_CREATE_ZONE_H
#define TPR_CHUNK_CREATE_ZONE_H

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_set>

//-------------------- Engine --------------------//
#include "tprCast.h"
#include "config.h"
#include "Chunk.h"
#include "tprAssert.h"


// only two instances in esrc:
// -- chunkCreateZone
// -- chunkReleaseZone
class ChunkZone{
public:
    explicit ChunkZone( int sideLen_ ): //- based on chunk
        sideLen(sideLen_),
        offLen(sideLen_/2),
        offLenMPos(offLen*ENTS_PER_CHUNK)
        {
            tprAssert( (sideLen_>0) && (sideLen_%2 != 0) );
            this->init_chunkMPoses();
        }

    inline void set_centerChunkMPos( const IntVec2 &chunkMPos_ ){
        this->centerChunkMCPos = chunkMPos_;
    }
    inline const IntVec2 &get_centerChunkMPos() const {
        return this->centerChunkMCPos;
    }
    inline const std::vector<IntVec2> &get_chunkOffMPoses() const {
        return this->chunkOffMPoses;
    }
    inline int get_offLenMPos() const {
        return this->offLenMPos;
    }

    inline size_t get_chunks() const {
        return cast_2_size_t(this->sideLen * this->sideLen);
    }

private:
    inline void init_chunkMPoses(){
        for( int h=-offLen; h<=offLen; h++ ){
            for( int w=-offLen; w<=offLen; w++ ){
                chunkOffMPoses.push_back( IntVec2{ w*ENTS_PER_CHUNK , h*ENTS_PER_CHUNK } );
            }
        }
    }
    //===== vals =====//
    int         sideLen     {}; //- alin to chunks
    int         offLen      {}; //- alin to chunks
    int         offLenMPos  {}; //- 半边长, alin to chunk-mpos
    IntVec2     centerChunkMCPos {}; //- center chunk:[left-bottom],alin to chunk-mpos

    std::vector<IntVec2> chunkOffMPoses {}; 
};



class ChunkCreateReleaseZone{
public:
    ChunkCreateReleaseZone( int sideLen_create_, 
                            int sideLen_release_ ): //- based on chunk
        createZoneUPtr( std::make_unique<ChunkZone>(sideLen_create_) ),
        releaseZoneUPtr( std::make_unique<ChunkZone>(sideLen_release_) ),
        LimitOff( std::abs(createZoneUPtr->get_offLenMPos() - releaseZoneUPtr->get_offLenMPos()) )
        {}


    void init( const IntVec2 &playerMPos_ );


    ChunkZone &get_createZoneRef(){
        return *(this->createZoneUPtr.get());
    }
    ChunkZone &get_releaseZoneRef(){
        return *(this->releaseZoneUPtr.get());
    }

    void refresh_and_release( const IntVec2 &playerMPos_ ); // major-func


private:
    std::unique_ptr<ChunkZone> createZoneUPtr;
    std::unique_ptr<ChunkZone> releaseZoneUPtr;

    int LimitOff;   //- 释放圈 比 新建圈 大出的 半边长，alin to chunk-mpos
                    // 当 两圈center间距离，大于此值，说明新建圈越界，要推动 释放圈了

    
    //-- 轮流扮演 old/new 容器，容纳 释放圈覆盖的所有 chunkkeys --
    std::unordered_set<chunkKey_t> releaseChunkKeys_A {};
    std::unordered_set<chunkKey_t> releaseChunkKeys_B {};

    //===== flags =====//
    bool is_currentOld_in_A   {};

};












#endif 

