/*
 * ============= check_and_build_sections_3.cpp =================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.20
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  - 3th
 * ----------------------------
 */

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <set>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "Chunk.h"
#include "sectionKey.h"
#include "esrc_field.h"
#include "esrc_chunk.h"
#include "esrc_player.h"
#include "esrc_shader.h"
#include "esrc_section.h"
#include "esrc_ecoSysInMap.h"

#include "sectionBuild.h"

//#include "debug.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


namespace{//----------- namespace ----------------//

    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 

    //----- funcs -----//
    void fst_sections_and_ecoSysInMaps( sectionKey_t _sectionKey );
    void collect_chunks_need_to_be_build_to_chunksDeque( chunkKey_t _chunkKey );

}//-------------- namespace : end ----------------//


void build_one_chunk_3( const IntVec2 &_anyMPos );
void build_new_chunk_in_update_3();


/* ===========================================================
 *                 build_9_chunks_3  [3th]
 * -----------------------------------------------------------
 * 游戏最开始，一口气 创建 周边9个 chunk
 * -------
 */
void build_9_chunks_3( const IntVec2 &_playerMPos ){

    IntVec2    playerChunkMPos = anyMPos_2_chunkMPos( _playerMPos );
    IntVec2    tmpChunkMPos;
    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            tmpChunkMPos.set(   playerChunkMPos.x + w*ENTS_PER_CHUNK,
                                playerChunkMPos.y + h*ENTS_PER_CHUNK );
            if( !esrc::find_from_chunks( chunkMPos_2_chunkKey(tmpChunkMPos) ) ){
                build_one_chunk_3( tmpChunkMPos );
            }
        }
    }
}


/* ===========================================================
 *    collect_chunks_need_to_be_build_in_update_3  [3th]
 * -----------------------------------------------------------
 * 在游戏运行时，定期检查 玩家位置。及时生成 新的 chunk
 * 确保，玩家周边 9个chunk 始终存在
 * -------
 */
void collect_chunks_need_to_be_build_in_update_3(){

    IntVec2 playerMPos = esrc::player.goPtr->goPos.get_currentMPos();

    currentChunkKey = anyMPos_2_chunkKey( playerMPos );
    if( is_first_check ){
        is_first_check = false;
        lastChunkKey = currentChunkKey;
        return;
    }
    if( currentChunkKey != lastChunkKey ){
        lastChunkKey = currentChunkKey;
        //build_9_chunks_3( playerMPos );
        collect_chunks_need_to_be_build_to_chunksDeque( currentChunkKey );
    }
}


/* ===========================================================
 *    collect_chunks_need_to_be_build_in_update_3  [3th]
 * -----------------------------------------------------------
 * 从全局容器 esrc::chunksDeque 取出一个元素，并 创建这个 chunk
 */
void build_one_chunks_from_chunksDeque(){

    if( esrc::is_chunksDeque_empty() ){
        return;
    }

    chunkKey_t key = esrc::pop_from_chunksDeque();
    build_one_chunk_3( chunkKey_2_mpos(key) );
}



/* ===========================================================
 *                   build_one_chunk_3  [第三版]
 * -----------------------------------------------------------
 * param: _anyMPos -- 目标chunk 中的 任何mpos
 */
void build_one_chunk_3( const IntVec2 &_anyMPos ){

            //   调用本函数，说明一定处于 “无视存储” 的早期阶段。

    IntVec2 targetChunkMPos = anyMPos_2_chunkMPos(_anyMPos);

    //------------------------------//
    //           [1]
    // 收集 周边 4个 sectionKey
    // 创建它们的 第一阶段数据 ( section / ecoSysInMap )
    //------------------------------//
    IntVec2  tmpChunkMPos;
    for( size_t h=0; h<=1; h++ ){
        for( size_t w=0; w<=1; w++ ){ //- 周边 4 个chunk
            tmpChunkMPos.set(   targetChunkMPos.x + w*ENTS_PER_CHUNK,
                                targetChunkMPos.y + h*ENTS_PER_CHUNK );
            fst_sections_and_ecoSysInMaps( anyMPos_2_sectionKey(tmpChunkMPos) );
        }
    }

    //------------------------------//
    //            [2]
    //  集中生成 周边 4chunk 的 所有 fields
    //------------------------------//
    IntVec2  tmpFieldMPos;
    for( size_t h=0; h<FIELDS_PER_CHUNK*2; h++ ){
        for( size_t w=0; w<FIELDS_PER_CHUNK*2; w++ ){ //- each field in 2*2chunks

            tmpFieldMPos.set(   targetChunkMPos.x + w*ENTS_PER_FIELD,
                                targetChunkMPos.y + h*ENTS_PER_FIELD );
            esrc::atom_try_to_insert_and_init_the_field_ptr( tmpFieldMPos );
        }
    }

    //------------------------------//
    //            [3]
    //    单独生成 主chunk 实例
    //------------------------------//
    Chunk *chunkPtr = esrc::insert_and_init_new_chunk( targetChunkMPos, &(esrc::rect_shader) );//- 一定不存在

    //------------------------------//
    //            [4]
    //  为 chunk 中的 8*8 个 field，分配 goes
    //------------------------------//
    for( const auto &fieldKey : chunkPtr->get_reseted_fieldKeys() ){ //- each field key
        sectionBuild::create_a_go_in_field( fieldKey );
    } //-- each field key end --

}



namespace{//----------- namespace ----------------//


/* ===========================================================
 *    collect_chunks_need_to_be_build_to_chunksDeque
 * -----------------------------------------------------------
 *  检查目标 chunk 周边9个chunk，如果哪个chunk 尚未生成，就将它 push到 全局容器 
 */
void collect_chunks_need_to_be_build_to_chunksDeque( chunkKey_t _chunkKey ){

    IntVec2      playerChunkMPos = chunkKey_2_mpos( _chunkKey );
    IntVec2      tmpChunkMPos;
    chunkKey_t   tmpChunkKey;
    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            tmpChunkMPos.set(   playerChunkMPos.x + w*ENTS_PER_CHUNK,
                                playerChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkKey = chunkMPos_2_chunkKey(tmpChunkMPos);
            if( !esrc::find_from_chunks(tmpChunkKey) ){
                esrc::push_to_chunksDeque( tmpChunkKey );
            }
        }
    }
}



/* ===========================================================
 *             fst_sections_and_ecoSysInMaps
 * -----------------------------------------------------------
 * 第一阶段
 */
void fst_sections_and_ecoSysInMaps( sectionKey_t _sectionKey ){

    Section     *tmpSectionPtr;
    bool         isNew {false};
    //------------------------------//
    // 检查 目标section 实例, 若没有，创建之
    // 检查 周边4个 ecosysinmap 实例，若没有，创建之
    //------------------------------//
    
    if( !esrc::atom_find_from_sections(_sectionKey) ){
        tmpSectionPtr = esrc::atom_insert_and_init_new_section( _sectionKey );
        isNew = true;
    }else{
        tmpSectionPtr = esrc::atom_get_sectionPtr( _sectionKey );
    }
    
    for( const auto &key : tmpSectionPtr->get_quadSectionKeys() ){ //- each quad sectionkey
        EcoSysInMap::find_or_create_the_ecoSysInMap( key );
    }

    //------------------------------//
    // 为 目标section 绑定 周边 4个 ecosysinmap 实例指针
    //------------------------------//
    if( isNew ){
        tmpSectionPtr->bind_ecoSysInMapPtrs();
    }
}




}//-------------- namespace : end ----------------//
}//----------------- namespace: sectionBuild: end -------------------//
