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

namespace sectionBuild { //------- namespace: sectionBuild ----------//


namespace{//----------- namespace ----------------//

    u64_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    u64_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key

    std::vector<Section*> nineSectionPtrs {}; //- 9个section指针 容器

    std::vector<chunkKey_t> nineSection_chunkKeys {}; //- 9*16 个 chunkKeys

    void create_a_chunk( const MapCoord &_chunkMCPos );

}//-------------- namespace : end ----------------//


/* ===========================================================
 *                   build_first_section  [第二版]
 * -----------------------------------------------------------
 * -- 生成 玩家所在的 第一个 section
 * -------
 * param: _playerMPos -- 玩家所在 mpos
 */
void build_first_section( const IntVec2 &_playerMPos ){

            //.. 二话不说直接创建 目标 chunk
            //   调用本函数，说明一定处于 “无视存储” 的早期阶段。
            //MapCoord chunkMCPos { anyMPos_2_chunkMPos(_playerMPos) };
            //create_a_chunk( chunkMCPos );


    nineSection_chunkKeys.clear();
    
    IntVec2 chunkMPos   = anyMPos_2_chunkMPos(_playerMPos);
    IntVec2 sectionMPos = anyMPos_2_sectionMPos(_playerMPos);

    sectionPtr = esrc::insert_new_section( sectionMPos );
    sectionPtr->init();

    //------------------------------//
    // 创建玩家所在的 section，及其周边共9个section 实例
    // 以及 16 个 ecoSysInMap 实例
    //------------------------------//
    nineSectionPtrs.clear();
    //-----
    Section     *tmpSectionPtr     {nullptr};
    EcoSysInMap *tmpEcoSysInMapPtr {nullptr};
    for( const auto &key : sectionPtr->get_nearbySectionKeys() ){
        if(esrc::sections.find(key) == esrc::sections.end()){
            tmpSectionPtr = esrc::insert_new_section(key);
            tmpSectionPtr->init();
        }else{
            tmpSectionPtr = esrc::get_sectionPtr(key);
        }
        nineSectionPtrs.push_back( tmpSectionPtr ); //- copy

        //-- 根据9个section的所有端点，创建 16个 ecoSysInMap实例 -- 
        for( const auto &key : tmpSectionPtr->get_quadSectionKeys() ){
            if(esrc::ecoSysesInMap.find(key) == esrc::ecoSysesInMap.end()){
                tmpEcoSysInMapPtr = esrc::insert_new_ecoSysInMap(key);
                tmpEcoSysInMapPtr->init();
            }else{
                tmpEcoSysInMapPtr = esrc::get_ecoSysInMapPtr(key);
            }
            //...

        }
    }


    //------------------------------//
    // 为 主section 绑定 4个端点的 ecosys 实例指针
    //------------------------------//
    sectionPtr->bind_ecoSysInMapPtrs();
       

    //------------------------------//
    // 为 9个 section 中的 chunks 分配  ecoSysInMap
    // 这些数据 暂时存储在 section 中
    //------------------------------//
    for( const auto &sPtr : nineSectionPtrs ){
        sPtr->assign_chunks_to_ecoSysInMap();
    }


    //------------------------------//
    //  主 section 相关的 4个 ecosysInMap 现在有了足够的数据
    //  可以开始做 “ecosys规划”
    //------------------------------//
    for( auto &ecoSysPtr : sectionPtr->ecoSysInMapPtrs ){
        ecoSysPtr->plan();
    }

        //exit(0); //- 之前无编译问题
        

    //  现在，准备阶段的数据似乎已经生成完毕
    //...
    

    //------------------------------//
    // 生成 9*16 个chunk 的 ChunkFieldSet 实例
    // 并 初始化 chunkFieldSet.fields 的一阶数据
    //------------------------------//
    IntVec2   tmpChunkMPos;
    for( const auto &sPtr : nineSectionPtrs ){ //- each section 
        const IntVec2 &sectionMPosRef = sPtr->mcpos.get_mpos();

        for( int h=0; h<CHUNKS_PER_SECTION; h++ ){
            for( int w=0; w<CHUNKS_PER_SECTION; w++ ){ //- each chunk

            tmpChunkMPos.set(   sectionMPosRef.x + w*ENTS_PER_CHUNK,
                                sectionMPosRef.y + h*ENTS_PER_CHUNK );
            esrc::insert_new_chunkFieldSet( tmpChunkMPos ); 
                            //- 顺带初始化完毕

            nineSection_chunkKeys.push_back( chunkMPos_2_key( tmpChunkMPos ) );
                            //- 顺带将 9*16 个chunk key 存储进临时容器， 方便下一次遍历。
            }
        }
    }

    //------------------------------//
    // 遍历 周边9个 chunkFieldSet 的 所有 field
    // 初始化 field 的二阶数据
    //------------------------------//
    for( auto &chunkFieldSetPtr : get_nearby_chunkFieldSetPtrs(chunkMPos) ){ //- each chunkFieldSetPtr
        for( auto &fieldPair : chunkFieldSetPtr->fields ){ //- each field
            fieldPair.second.init_secondOrderData();
        }
    }
    


    //------------------------------//
    //  为 主chunk 的所有 field 做 cell-noise 分配
    //------------------------------//
    esrc::get_chunkFieldSetPtr( chunkMPos_2_key(chunkMPos) )->assign_fields_2_chunk();

    //... 现在，主chunk 的 每个 field 都知道自己的 ecosys 信息了 ...


    //------------------------------//
    //  单独生成 玩家所在的 chunk 实例
    //------------------------------//
    Chunk  *currentChunkPtr = esrc::insert_new_chunk( chunkMPos );//- 一定不存在
    size_t  chunkIdx = sectionPtr->get_chunk_idx( chunkMPos ); //- 本chunk 在 主section 中的序号
    //------
    currentChunkPtr->init_memMapEnts(); //- 填满 chunk.memMapEnts
    currentChunkPtr->nodePPos = sectionPtr->get_chunkNodePPos( chunkIdx ); //- copy nodePPos
    currentChunkPtr->ecoSysInMapKey = sectionPtr->get_chunkEcoSysInMapKey( chunkIdx ); //- copy ecoSysInMapKey
    //...


    //------------------------------//
    //  
    //------------------------------//



    //------------------------------//
    //  
    //------------------------------//


    //...



    exit(0);

}


/* ===========================================================
 *                   build_first_section
 * -----------------------------------------------------------
 * -- 创建目标 section 周边8个 section
 * -- 在游戏启动阶段，调用 build_first_section 后立即调用
 */
/*
void build_nearby_sections( const IntVec2 &_mpos ){

    //.. 二话不说直接创建 目标 chunk 周围8个 chunk
    //   调用本函数，说明一定处于 “无视存储” 的早期阶段。
    u64_t key = anyMPos_2_chunkKey( _mpos );

        assert( esrc::chunks.find(key) != esrc::chunks.end() ); //- tmp

    const auto &keys = esrc::chunks.at(key).get_near_9_chunkKeys();
    for( const auto &ikey : keys ){
        if( esrc::chunks.find(ikey) == esrc::chunks.end() ){
            //-- 创建 这个 chunk
            create_a_chunk( chunkKey_2_mcpos(ikey) );
        }
    }
}
*/



/* ===========================================================
 *                   check_and_build_nearby_sections
 * -----------------------------------------------------------
 * -- 根据传入的 pos，检查其周围 8个section。如果哪个不存在，就生成之。
 * -------
 * 一个非常临时性的函数，完全不考虑 存档实现。
 * 用于 早期 生态群落建设。
 */
/*
void check_and_build_nearby_sections(){

    IntVec2 playerMPos = esrc::player.goPtr->goPos.get_currentMPos();

    //-- 没有变更 chunk 时，直接终止检测 ---
    currentChunkKey = anyMPos_2_chunkKey( playerMPos );
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
*/


namespace{//----------- namespace ----------------//

/* ===========================================================
 *                     create_a_chunk
 * -----------------------------------------------------------
 * -- tmp，快速制作一个 section
 * -- 有关 “创建一个section”，现在出现了 多层嵌套...
 * -------
 * param: _chunkMCPos -- chunk 左下角mapent mcpos （强制）
 */
/*
void create_a_chunk( const MapCoord &_chunkMCPos ){

    Chunk *chunkPtr = esrc::insert_new_chunk( _chunkMCPos.get_mpos() );

    //---- 填满 mapChunk.memMapEnts ----
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            MemMapEnt mapEnt {};
            mapEnt.mcpos = chunkPtr->get_mcpos() + MapCoord{ w, h };
            chunkPtr->memMapEnts.push_back( mapEnt ); //-copy
        }
    }

    //----- 正式生成 section 数据 -----//
    //  仅仅生成数据，
    build_data( chunkPtr ); 


    //----- 正式生成 某张chunk.mapTex, -----//
    //...


    //-- bind mapTex - mesh --
    chunkPtr->mesh.init( chunkPtr->mapTex.get_texName() );
    chunkPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    chunkPtr->mesh.isVisible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    chunkPtr->refresh_translate_auto();
    chunkPtr->init();

}
*/

}//-------------- namespace : end ----------------//


}//----------------- namespace: sectionBuild: end -------------------//

