/*
 * =================== chunkRelease.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "chunkRelease.h"


//-------------------- Engine --------------------//
#include "Chunk.h"
#include "esrc_gameObj.h"
#include "esrc_player.h"
#include "esrc_field.h"
#include "esrc_chunk.h"

namespace chunkRelease {//------- namespace: chunkRelease ----------//
namespace cr_inn {//----------- namespace: cr_inn ----------------//

    void quit_edgeGos_from_mapEnt( Chunk &chunkRef_ );

}//-------------- namespace: cr_inn end ----------------//



/* ===========================================================
 *       collect_chunks_need_to_be_release_in_update
 * -----------------------------------------------------------
 */
void collect_chunks_need_to_be_release_in_update(){

    const auto &playerDPosRef = esrc::get_player().get_goRef().get_dpos();
    esrc::get_chunkCreateReleaseZoneRef().refresh_and_collect_chunks_need_to_be_release( dpos_2_mpos(playerDPosRef) );
}


/* ===========================================================
 *                 release_one_chunk
 * -----------------------------------------------------------
 * 直接在 main thread 执行
 */
void release_one_chunk(){

    //-------------------------------//
    //  检查 esrc::chunkKeys_waitForRelease
    //  若为空，直接退出
    //------------------------------//
    auto chunkKeyOpt = esrc::pop_front_from_WaitForRelease_and_move_2_onReleasing();
    if( !chunkKeyOpt.has_value() ){
        return;
    }

    chunkKey_t chunkKey = chunkKeyOpt.value();
    Chunk &chunkRef = esrc::get_chunkRef_onReleasing( chunkKey ); 
    IntVec2 chunkMPos = chunkKey_2_mpos(chunkKey);
    
    //------------------------------//
    //         gameObjs
    // 清除 go 在 map 上的登记
    // 部分 go，需要存入 db，剩余go直接删除
    //------------------------------//

    //-- 将部分go 存入 db --
    // 未实现...

    cr_inn::quit_edgeGos_from_mapEnt( chunkRef );

    //-- 删除本chunk 的所有 go 实例 --
    for( auto &goid : chunkRef.get_goIds() ){//- foreach goIds
        
        tprAssert( goid != esrc::get_player().playerGoCircle_goid );
        esrc::erase_the_go( goid );
    }
                    // 在未来，go 会向 chunk一样，拥有完整的 生命周期管理
                    // 同时，创建 go的释放队列。 延迟释放，平衡每帧的开销
                    // ...

    //------------------------------//
    //         fields
    // 删除所有 field 实例
    //------------------------------//
    esrc::erase_all_fields_in_chunk( chunkMPos );

    //------------------------------//
    //         chunk
    // 删除 chunk 实例本身
    //------------------------------//
    esrc::erase_from_chunks( chunkKey );

    //------------------------------//
    //          ecoObj
    // 目前方案：永不删除，就留在那
    //------------------------------//
    //...
}

namespace cr_inn {//----------- namespace: cr_inn ----------------//


/* 遍历 目标chunk 的 edgeGos，将它们在 周边chunk 的 mapent 上的所有 signUp，都注销
 * edgeGo 在本chunk 上的登记，以及 非edgeGo 在本chunk 上的登记，就不用注销了
 * 这些会在后面 一股脑 统一删除
 */
void quit_edgeGos_from_mapEnt( Chunk &chunkRef_ ){

    chunkKey_t chunkKey = chunkRef_.get_key();

    chunkKey_t tmpChunkKey  {};
    for( auto &goid : chunkRef_.get_edgeGoIds() ){//- foreach edgeGoId

        // get go ref
        auto goOpt = esrc::get_goPtr( goid );
        tprAssert( goOpt.has_value() );
        GameObj &goRef = *goOpt.value();

        auto colliderType = goRef.get_colliderType();
        if( colliderType == ColliderType::Circular ){
            // only MajorGo
            for( const auto &mpos : goRef.get_collisionRef().get_current_signINMapEnts_circle_ref() ){
                tmpChunkKey = anyMPos_2_chunkKey(mpos);
                if( (chunkKey!=tmpChunkKey) && (esrc::get_chunkMemState(tmpChunkKey)==ChunkMemState::Active) ){
                    // mp所在chunk 非本chunk，且 Active
                    //---- 正式从 mapEnt 上清除登记 -----
                    auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
                    tprAssert( mapEntPair.first == ChunkMemState::Active );
                    mapEntPair.second->erase_from_circular_goids( goRef.goid, goRef.get_colliderType() );
                }
            }

        }else if( colliderType == ColliderType::Square ){
            // MajorGo / BioSoupGo
            IntVec2 mpos {};
            IntVec2 goRootMPos = dpos_2_mpos( goRef.get_dpos() );
            for( const auto &mposOff : goRef.get_signInMapEnts_square_ref().get_all_mapEntOffs() ){
                mpos = goRootMPos + mposOff;
                
                tmpChunkKey = anyMPos_2_chunkKey(mpos);
                if( (chunkKey!=tmpChunkKey) && (esrc::get_chunkMemState(tmpChunkKey)==ChunkMemState::Active) ){
                    // mp所在chunk 非本chunk，且 Active
                    //---- 正式从 mapEnt 上清除登记 -----
                    auto mapEntPair = esrc::getnc_memMapEntPtr( mpos );
                    tprAssert( mapEntPair.first == ChunkMemState::Active );

                    if( goRef.family == GameObjFamily::Major ){
                        mapEntPair.second->erase_square_goid( goRef.goid, goRef.get_colliderType() );
                    }else if( goRef.family == GameObjFamily::BioSoup ){
                        mapEntPair.second->erase_bioSoup_goid( goRef.goid, goRef.family );
                    }else{
                        tprAssert(0);
                    }
                }
            }
        }else{
            tprAssert(0);
        }

    }
}



}//-------------- namespace: cr_inn end ----------------//
}//----------------- namespace: chunkRelease: end -------------------//

