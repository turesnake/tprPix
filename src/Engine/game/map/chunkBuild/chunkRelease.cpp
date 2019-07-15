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
#include "esrc_colliEntSet.h"



namespace chunkRelease {//------- namespace: chunkRelease ----------//
namespace cr_inn {//----------- namespace: cr_inn ----------------//


    void quit_edgeGos_from_mapEnt( Chunk &chunkRef_, chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ );


}//-------------- namespace: cr_inn end ----------------//



/* ===========================================================
 *       collect_chunks_need_to_be_release_in_update
 * -----------------------------------------------------------
 */
void collect_chunks_need_to_be_release_in_update(){

    IntVec2 playerMPos = esrc::get_player().get_goRef().goPos.get_currentMPos();
    esrc::get_chunkCreateReleaseZoneRef().refresh_and_collect_chunks_need_to_be_release( playerMPos );
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

    //------------------------------//
    //        chunk.texture
    // 删除 renderpool 中的 mesh
    // 删除 opengl 中的 有关对象
    //------------------------------//

    //-- mesh --
    // 每一渲染帧，都会重新 insert 所有 Active-chunk 的 mesh 到 renderPool
    // 所以，只要将 chunk 改为 WaitForRelease,
    // 就会 停止渲染它

    //- opengl texture --
    chunkRef.delete_mapTex();

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
void quit_edgeGos_from_mapEnt( Chunk &chunkRef_, chunkKey_t chunkKey_, const IntVec2 &chunkMPos_ ){

    IntVec2    cesMPos      {}; //- rootCES left-bottom mcpos [world-abs-pos]
    IntVec2    colliEntMPos {}; //- each collient mcpos [world-abs-pos]
    chunkKey_t tmpChunkKey  {};

    for( auto &goid : chunkRef_.get_edgeGoIds() ){//- foreach edgeGoId

        auto &goRef = esrc::get_goRef(goid);
        const ColliEntHead &cehRef = goRef.get_rootColliEntHeadRef();
        const ColliEntSet &cesRef = esrc::get_colliEntSetRef( cehRef.colliEntSetIdx );
        cesMPos = goRef.get_rootCES_leftBottom_MPos();

        for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in rootCES

            colliEntMPos = i.get_mpos() + cesMPos;
            tmpChunkKey = anyMPos_2_chunkKey(colliEntMPos);
            if( chunkKey_ != tmpChunkKey ){ // 只释放 非本chunk 的

                if( esrc::get_chunkMemState(tmpChunkKey) == ChunkMemState::Active ){
                    //---- 正式从 mapEnt 上清除登记 -----
                    auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( colliEntMPos );
                    mapEntRef.erase_from_major_gos( goRef.id );
                }
            }
        }
    }
}











}//-------------- namespace: cr_inn end ----------------//
}//----------------- namespace: chunkRelease: end -------------------//

