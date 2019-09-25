/*
 * =================== chunkRelease.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "chunkRelease.h"


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Chunk.h"
#include "esrc_gameObj.h"
#include "esrc_player.h"
#include "esrc_field.h"
#include "esrc_chunk.h"



namespace chunkRelease {//------- namespace: chunkRelease ----------//
namespace cr_inn {//----------- namespace: cr_inn ----------------//


    void quit_edgeGos_from_mapEnt( Chunk &chunkRef_, chunkKey_t chunkKey_, IntVec2 chunkMPos_ );


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
 */
void release_one_chunk(){

    //-------------------------------//
    //  检查 esrc::chunkKeys_waitForRelease
    //  若为空，直接退出
    //------------------------------//
    auto popRet = esrc::pop_front_from_WaitForRelease_and_move_2_onReleasing();
    if( popRet.first == false ){
        return;
    }

    chunkKey_t chunkKey = popRet.second;
    auto &chunkRef = esrc::get_chunkRef_onReleasing( chunkKey ); 
    IntVec2 chunkMPos = chunkKey_2_mpos(chunkKey);
    
    //------------------------------//
    //         gameObjs
    // 清除 go 在 map 上的登记
    // 部分 go，需要存入 db，剩余go直接删除
    //------------------------------//

    //-- 将部分go 存入 db --
    // 未实现...

    cr_inn::quit_edgeGos_from_mapEnt( chunkRef, chunkKey, chunkMPos );

    //-- 删除本chunk 的所有 go 实例 --
    for( auto &goid : chunkRef.get_goIds() ){//- foreach goIds
        esrc::erase_the_go( goid );
    }
                    // 在未来，go 会向 chunk一样，拥有完整的 生命周期管理
                    // 同时，创建 go的释放队列。 延迟释放，平均每帧的开销
                    // ...

    //------------------------------//
    //         fields
    // 删除所有 field 实例
    //------------------------------//
    esrc::atom_erase_all_fields_in_chunk( chunkMPos );

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


/* ===========================================================
 *                quit_edgeGos_from_mapEnt
 * -----------------------------------------------------------
 * 遍历 目标chunk 的 edgeGos，将它们在 周边chunk 的 mapent 上的所有 signUp，都注销
 * edgeGo 在本chunk 上的登记，以及 非edgeGo 在本chunk 上的登记，就不用注销了
 * 在后面 一股脑删除即可
 */
void quit_edgeGos_from_mapEnt( Chunk &chunkRef_, chunkKey_t chunkKey_, IntVec2 chunkMPos_ ){

    chunkKey_t tmpChunkKey  {};
    //--
    for( auto &goid : chunkRef_.get_edgeGoIds() ){//- foreach edgeGoId

        auto &goRef = esrc::get_goRef(goid);

        for( const auto &mpos : goRef.get_currentSignINMapEntsRef() ){
            tmpChunkKey = anyMPos_2_chunkKey(mpos);
            if( chunkKey_ != tmpChunkKey ){ // 只释放 非本chunk 的

                if( esrc::get_chunkMemState(tmpChunkKey) == ChunkMemState::Active ){
                    //---- 正式从 mapEnt 上清除登记 -----
                    auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( mpos );
                    mapEntRef.erase_the_onlyOne_from_majorGos( goRef.id );
                }
            }
        }
    }
}



}//-------------- namespace: cr_inn end ----------------//
}//----------------- namespace: chunkRelease: end -------------------//

