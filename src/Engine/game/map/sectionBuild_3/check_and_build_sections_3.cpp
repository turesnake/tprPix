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
#include "sectionBuild_inn.h"


#include "debug.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


namespace{//----------- namespace ----------------//

    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 

    //-- 要生成的那个 chunk --
    IntVec2       targetChunkMPos;
    IntVec2       targetSectionMPos;
    chunkKey_t    targetChunkKey;
    sectionKey_t  targetSectionKey;
    Chunk        *targetChunkPtr;



    std::vector<chunkKey_t> nearFour_chunkKeys   {}; //- 主chunk 周边4个 chunk key 容器
    std::set<sectionKey_t>  nearFour_sectionKeys {}; //- 主4chunks 相关的 所有 section key [1,4]
                                                     //- 核心数据

    //----- funcs -----//
    void collect_nearFour_chunkKeys_and_sectionKeys();
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
            if( esrc::chunks.find(chunkMPos_2_chunkKey(tmpChunkMPos)) == esrc::chunks.end()  ){
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

    if( esrc::is_chunksDeque_empty() == true ){
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

    targetChunkMPos   = anyMPos_2_chunkMPos(_anyMPos);
    targetSectionMPos = anyMPos_2_sectionMPos(_anyMPos);
    targetChunkKey   = chunkMPos_2_chunkKey(targetChunkMPos);
    targetSectionKey = sectionMPos_2_sectionKey(targetSectionMPos);

    //------------------------------//
    // 收集 周边4chunk，以及这些chunk所属的 section信息 到临时容器中
    // 检查这些相关 section，若没有，创建之。
    //------------------------------//
    collect_nearFour_chunkKeys_and_sectionKeys();

    //------------------------------//
    // 遍历 每一个 “相关section”，创建它们的 第一阶段数据
    //------------------------------//
    for( const auto &sectionKey : nearFour_sectionKeys ){ //- each key
        fst_sections_and_ecoSysInMaps( sectionKey );
    }

    //------------------------------//
    //          第2阶段
    //    单独生成 主chunk 实例
    //------------------------------//
    targetChunkPtr = esrc::insert_new_chunk( targetChunkMPos );//- 一定不存在
    targetChunkPtr->init();

    //------------------------------//
    //  为chunk中 每个 mapent/pix 分配对应的 field
    //------------------------------//
    targetChunkPtr->assign_ents_and_pixes_to_field();

    //------------------------------//
    //  主chunk mesh 初始化
    //------------------------------//
    //-- bind mapTex - mesh --
    targetChunkPtr->mesh.init( targetChunkPtr->mapTex.get_texName() );
    targetChunkPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    targetChunkPtr->mesh.isVisible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    targetChunkPtr->refresh_translate_auto();

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
            if( esrc::chunks.find(tmpChunkKey) == esrc::chunks.end()  ){
                esrc::push_to_chunksDeque( tmpChunkKey );
            }
        }
    }
}



/* ===========================================================
 *       collect_nearFour_chunkKeys_and_sectionKeys
 * -----------------------------------------------------------
 */
void collect_nearFour_chunkKeys_and_sectionKeys(){

    IntVec2       tmpChunkMPos;
    chunkKey_t    tmpChunkKey;
    sectionKey_t  tmpSectionKey;

    nearFour_chunkKeys.clear();
    nearFour_sectionKeys.clear();

    for( int h=0; h<=1; h++ ){
        for( int w=0; w<=1; w++ ){ //- 周边 4 个chunk
            tmpChunkMPos.set(   targetChunkMPos.x + w*ENTS_PER_CHUNK,
                                targetChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkKey = chunkMPos_2_chunkKey( tmpChunkMPos );
            nearFour_chunkKeys.push_back( tmpChunkKey );
            //--------
            tmpSectionKey = anyMPos_2_sectionKey( tmpChunkMPos );
            nearFour_sectionKeys.insert( tmpSectionKey );
        }
    }
}


/* ===========================================================
 *             fst_sections_and_ecoSysInMaps
 * -----------------------------------------------------------
 * 第一阶段
 */
void fst_sections_and_ecoSysInMaps( sectionKey_t _sectionKey ){

    IntVec2      sectionMPos = sectionKey_2_mpos( _sectionKey );
    Section     *tmpSectionPtr;
    EcoSysInMap *tmpEcoSysInMapPtr;

    //------------------------------//
    // 检查 目标section 实例, 若没有，创建之
    // 检查 周边4个 ecosysinmap 实例，若没有，创建之
    //------------------------------//
    if( esrc::sections.find(_sectionKey) == esrc::sections.end() ){
        tmpSectionPtr = esrc::insert_new_section( sectionMPos );
        tmpSectionPtr->init();
    }else{
        tmpSectionPtr = esrc::get_sectionPtr( _sectionKey );
    }

    for( const auto &key : tmpSectionPtr->get_quadSectionKeys() ){ //- each quad sectionkey
        if(esrc::ecoSysesInMap.find(key) == esrc::ecoSysesInMap.end()){
            tmpEcoSysInMapPtr = esrc::insert_new_ecoSysInMap(key);
            tmpEcoSysInMapPtr->init();
            tmpEcoSysInMapPtr->plan();
        }
    }

    //------------------------------//
    // 为 目标section 绑定 周边 4个 ecosysinmap 实例指针
    //------------------------------//
    tmpSectionPtr->bind_ecoSysInMapPtrs();


}




}//-------------- namespace : end ----------------//
}//----------------- namespace: sectionBuild: end -------------------//
