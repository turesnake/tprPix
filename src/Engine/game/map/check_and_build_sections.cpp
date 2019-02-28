/*
 * ============= check_and_build_sections.cpp =================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  - tmp 
 *    
 * ----------------------------
 */
#include "Chunk.h"
#include "srcs_engine.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "sectionBuild.h"

#include "debug.h"

namespace{//----------- namespace ----------------//

    u64_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    u64_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key

    void create_a_section( const MapCoord &_chunkMCPos );

}//-------------- namespace : end ----------------//


/* ===========================================================
 *                   build_first_section
 * -----------------------------------------------------------
 * -- 生成 玩家所在的 第一个 section
 * -------
 * 一个非常临时性的函数，完全不考虑 存档实现。
 * 用于 早期 生态群落建设。
 * ----
 * param: _mpos -- 玩家所在 mpos
 */
void build_first_section( const IntVec2 &_mpos ){

    //.. 二话不说直接创建 目标 chunk
    //   调用本函数，说明一定处于 “无视存储” 的早期阶段。
    MapCoord chunkMCPos { get_chunk_mpos(_mpos) };
    create_a_section( chunkMCPos );
}


/* ===========================================================
 *                   build_first_section
 * -----------------------------------------------------------
 * -- 创建目标 section 周边8个 section
 */
void build_nearby_sections( const IntVec2 &_mpos ){

    //.. 二话不说直接创建 目标 chunk 周围8个 chunk
    //   调用本函数，说明一定处于 “无视存储” 的早期阶段。
    u64_t key = just_creat_chunkKey_by_mpos( _mpos );

        assert( esrc::chunks.find(key) != esrc::chunks.end() ); //- tmp

    const auto &keys = esrc::chunks.at(key).get_near_9_chunkKeys();
    for( const auto &ikey : keys ){
        if( esrc::chunks.find(ikey) == esrc::chunks.end() ){
            //-- 创建 这个 chunk
            create_a_section( chunkKey_2_mcpos(ikey) );
        }
    }
}



/* ===========================================================
 *                   check_and_build_nearby_sections
 * -----------------------------------------------------------
 * -- 根据传入的 pos，检查其周围 8个section。如果哪个不存在，就生成之。
 * -------
 * 一个非常临时性的函数，完全不考虑 存档实现。
 * 用于 早期 生态群落建设。
 */
void check_and_build_nearby_sections(){

    IntVec2 playerMPos = esrc::player.goPtr->goPos.get_currentMPos();

    //-- 没有变更 chunk 时，直接终止检测 ---
    currentChunkKey = just_creat_chunkKey_by_mpos( playerMPos );
    if( currentChunkKey == lastChunkKey ){
        return;
    }
    lastChunkKey = currentChunkKey;

    //-------------------------//
    // 检测并 创建 周边 8 个 chunk
    // 默认 自身所处 chunk 已经创建了
    //-------------------------//
    build_nearby_sections( playerMPos );
}


namespace{//----------- namespace ----------------//

/* ===========================================================
 *                     create_a_section
 * -----------------------------------------------------------
 * -- tmp，快速制作一个 section
 * -- 有关 “创建一个section”，现在出现了 多层嵌套...
 * -------
 * param: _chunkMCPos -- chunk 左下角mapent mcpos （强制）
 */
void create_a_section( const MapCoord &_chunkMCPos ){

    Chunk *chunkPtr = esrc::insert_new_chunk( _chunkMCPos );

    //---- 填充满 mapChunk.memMapEnts ----
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            MemMapEnt mapEnt {};
            mapEnt.mcpos = chunkPtr->get_mcpos() + MapCoord{ w, h };
            chunkPtr->memMapEnts.push_back( mapEnt ); //-copy
        }
    }

    //----- 正式生成 chunk 数据 -----//
    sectionBuild::build( chunkPtr ); //- 此时目标 chunk 是空的

    //-- bind mapTex - mesh --
    chunkPtr->mesh.init( chunkPtr->mapTex.get_texName() );
    chunkPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    chunkPtr->mesh.isVisible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    chunkPtr->refresh_translate_auto();
    chunkPtr->init();

}

}//-------------- namespace : end ----------------//

