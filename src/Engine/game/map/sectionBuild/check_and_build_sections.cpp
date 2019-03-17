/*
 * ============= check_and_build_sections.cpp =================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  - tmp 
 * ----------------------------
 */

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <set>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "sectionBuild_innerDatas.h"


#include "debug.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


namespace{//----------- namespace ----------------//

    chunkKey_t   lastChunkKey    {}; //- 上次检测时，记录的 玩家所在 chunk.key
    chunkKey_t   currentChunkKey {}; //- 此次检测时，玩家所在 chunk.key
    bool         is_first_check  {true}; 

    std::vector<chunkKey_t> nineChunkKeys      {}; //- 主chunk 周边9个 chunk key 容器
    std::set<sectionKey_t>  relatedSectionKeys {}; //- 主9chunks 相关的 所有 section key 
                                                   //- 核心数据

    std::unordered_map<sectionKey_t,std::vector<Section*>> all_nineSectionPtrs {};
                                        //- 每个 “相关section” 占有一份，
                                        //  每一份，存储某 “相关section” 的 周边9个 section 指针
                                        
    std::vector<ChunkFieldSet*> nearby_chunkFieldSetPtrs {}; //- 周边9个 ChunkFieldSet 实例指针
                                                    //- 被每个 chunk 反复使用

    

    //======= funcs =======//
    void reset_nineChunkKeys( const IntVec2 &_currentChunkMPos );
    void reset_relatedSectionKeys();
    void fst_sections_and_ecoSysInMaps( sectionKey_t _sectionKey );
    void sec_chunkFieldSet( chunkKey_t _chunkKey );
    std::vector<ChunkFieldSet*> &get_nearby_chunkFieldSetPtrs( const IntVec2 &_anyMPos );
    void build_landWaterPrefabDatas_for_each_section( const IntVec2 &_sectionMPos );

}//-------------- namespace : end ----------------//



void build_one_chunk( const IntVec2 &_anyMPos );
void build_new_chunk_in_update();




/* ===========================================================
 *                   build_one_chunk  [第二版]
 * -----------------------------------------------------------
 * 游戏最开始，一口气 创建 周边9个 chunk
 * -------
 */
void build_9_chunks( const IntVec2 &_playerMPos ){

    IntVec2    playerChunkMPos = anyMPos_2_chunkMPos( _playerMPos );
    IntVec2    tmpChunkMPos;
    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            tmpChunkMPos.set(   playerChunkMPos.x + w*ENTS_PER_CHUNK,
                                playerChunkMPos.y + h*ENTS_PER_CHUNK );
            if( esrc::chunks.find(anyMPos_2_chunkKey(tmpChunkMPos)) == esrc::chunks.end()  ){
                build_one_chunk( tmpChunkMPos );
            }
        }
    }
}



/* ===========================================================
 *                   build_one_chunk  [第二版]
 * -----------------------------------------------------------
 * 在游戏运行时，定期检查 玩家位置。及时生成 新的 chunk
 * 确保，玩家周边 9个chunk 始终存在
 * -------
 */
void build_new_chunk_in_update(){

    IntVec2 playerMPos = esrc::player.goPtr->goPos.get_currentMPos();


    currentChunkKey = anyMPos_2_chunkKey( playerMPos );

    if( is_first_check ){
        is_first_check = false;
        lastChunkKey = currentChunkKey;
        return;
    }

    if( currentChunkKey != lastChunkKey ){
        lastChunkKey = currentChunkKey;
        build_9_chunks( playerMPos );
    }
}




/* ===========================================================
 *                   build_one_chunk  [第二版]
 * -----------------------------------------------------------
 * param: _anyMPos -- 目标chunk 中的 任何mpos
 */
void build_one_chunk( const IntVec2 &_anyMPos ){

    //   调用本函数，说明一定处于 “无视存储” 的早期阶段。

    all_nineSectionPtrs.clear();
    
    IntVec2       currentChunkMPos   = anyMPos_2_chunkMPos(_anyMPos);
    IntVec2       currentSectionMPos = anyMPos_2_sectionMPos(_anyMPos);
    sectionKey_t  currentSectionKey = anyMPos_2_sectionKey(currentSectionMPos);

    //------------------------------//
    // 检查 主section 实例, 若没有，创建之
    //------------------------------//
    if( esrc::sections.find(currentSectionKey) == esrc::sections.end() ){
        currentSectionPtr = esrc::insert_new_section( currentSectionMPos );
        currentSectionPtr->init();
    }else{
        currentSectionPtr = esrc::get_sectionPtr( currentSectionKey );
    }


    //------------------------------//
    //  收集 主chunk 周边9个chunk。
    //  收集 这些chunk 所属的 section key [1,4] 这些 sections 都需要被 完全建设
    //------------------------------//
    reset_nineChunkKeys( currentChunkMPos );
    reset_relatedSectionKeys();


    //------------------------------//
    // 遍历 每一个 “相关section”，创建它们的 第一阶段数据
    //------------------------------//
    for( const auto &sectionKey : relatedSectionKeys ){ //- each key
        fst_sections_and_ecoSysInMaps( sectionKey );
    }

    // 现在，主9chunk 中的每个 chunk，所处的 section，都是一个 “完备section”
    // 意为：这个section 相接的 4个 ecosysinmap 实例，都是 完成体。
    // 


    //------------------------------//
    // 遍历 主chunk 周边9个chunk， 创建 第二阶段数据 
    //------------------------------//
    for( auto &chunkKey : nineChunkKeys ){ //- each chunk key in nearby-9
        sec_chunkFieldSet( chunkKey );
    }


    //------------------------------//
    //          第三阶段
    //    单独生成 主chunk 实例
    //------------------------------//
    currentChunkPtr = esrc::insert_new_chunk( currentChunkMPos );//- 一定不存在
    size_t  chunkIdx = currentSectionPtr->get_chunk_idx( currentChunkMPos ); //- 本chunk 在 主section 中的序号
    //------
    currentChunkPtr->init_memMapEnts(); //- 填满 chunk.memMapEnts
    currentChunkPtr->acquire_landWaterEnts_from_esrc(); //- 获得 landWater 数据
    currentChunkPtr->nodeMPos = currentSectionPtr->get_chunkNodeMPos( chunkIdx ); //- copy nodeMPos
    currentChunkPtr->ecoSysInMapKey = currentSectionPtr->get_chunkEcoSysInMapKey( chunkIdx ); //- copy ecoSysInMapKey
    //...


    //------------------------------//
    //  为 主chunk的 所有 mapent 分配 field
    //------------------------------//
    currentChunkPtr->assign_mapEnts_2_field();


    //------------------------------//
    //  使用 最简模式 来为 主chunk 生成 maptex
    //------------------------------//
    currentChunkPtr->assign_pixels_2_mapent();
                //- 调用这一步时，应该确保 周边9个chunk 都执行了 assign_mapEnts_2_field()。

    //...

    //------------------------------//
    //  主chunk mesh 初始化
    //------------------------------//
    //-- bind mapTex - mesh --
    currentChunkPtr->mesh.init( currentChunkPtr->mapTex.get_texName() );
    currentChunkPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    currentChunkPtr->mesh.isVisible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    currentChunkPtr->refresh_translate_auto();
    currentChunkPtr->init();

}




namespace{//----------- namespace ----------------//


/* ===========================================================
 *                   reset_nineChunkKeys
 * -----------------------------------------------------------
 * 收集 主chunk 周边9个 chunk key
 */
void reset_nineChunkKeys( const IntVec2 &_currentChunkMPos ){
    IntVec2      tmpChunkMPos;
    nineChunkKeys.clear();
    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
        tmpChunkMPos.set(   _currentChunkMPos.x + w*ENTS_PER_CHUNK,
                            _currentChunkMPos.y + h*ENTS_PER_CHUNK );
        nineChunkKeys.push_back( anyMPos_2_chunkKey(tmpChunkMPos) );
        }
    }
    assert( nineChunkKeys.size() == 9 );
}


/* ===========================================================
 *                 reset_relatedSectionKeys
 * -----------------------------------------------------------
 *  遍历 主chunk 周边9个 chunk，收集它们所属的 所有 section key
 *  这些 section，就是 在第一阶段 要建设完备的 section
 */
void reset_relatedSectionKeys(){
    IntVec2        tmpMpos;
    sectionKey_t   tmpSectionKey;
    relatedSectionKeys.clear();
    for( const auto &chunkKey : nineChunkKeys ){ //- each chunk key
        tmpMpos = chunkKey_2_mpos( chunkKey );
        tmpSectionKey = anyMPos_2_sectionKey( tmpMpos );
        relatedSectionKeys.insert( tmpSectionKey ); //- 自动去除多余
    }
}



/* ===========================================================
 *                  fst_sections_and_ecoSysInMaps
 * -----------------------------------------------------------
 * 第一阶段
 */
void fst_sections_and_ecoSysInMaps( sectionKey_t _sectionKey ){

    IntVec2   sectionMPos = sectionKey_2_mpos( _sectionKey );
    Section  *sectionPtr;
    
    //------------------------------//
    // 检查 目标section 实例, 若没有，创建之
    //------------------------------//
    if( esrc::sections.find(_sectionKey) == esrc::sections.end() ){
        sectionPtr = esrc::insert_new_section( sectionMPos );
        sectionPtr->init();
        build_landWaterPrefabDatas_for_each_section( sectionMPos );
    }else{
        sectionPtr = esrc::get_sectionPtr( _sectionKey );
    }

    //------------------------------//
    // 检查 目标section 周边9个 section 实例，若没有，创建之
    // 检查 16 个 ecoSysInMap 实例，若没有，创建之
    //------------------------------//
    all_nineSectionPtrs.insert({ _sectionKey, std::vector<Section*>{} }); // ???

    Section     *tmpSectionPtr     {nullptr};
    EcoSysInMap *tmpEcoSysInMapPtr {nullptr};
    for( const auto &key : sectionPtr->get_nearbySectionKeys() ){ //- each nearby 9 sectionkey
        if(esrc::sections.find(key) == esrc::sections.end()){
            tmpSectionPtr = esrc::insert_new_section(key);
            tmpSectionPtr->init();
            build_landWaterPrefabDatas_for_each_section( sectionMPos );
        }else{
            tmpSectionPtr = esrc::get_sectionPtr(key);
        }
        all_nineSectionPtrs.at(_sectionKey).push_back( tmpSectionPtr ); //- copy
        //--- 此 section 周边4个 ecosysinmap 实例 ---
        for( const auto &key : tmpSectionPtr->get_quadSectionKeys() ){ //- each quad sectionkey
            if(esrc::ecoSysesInMap.find(key) == esrc::ecoSysesInMap.end()){
                tmpEcoSysInMapPtr = esrc::insert_new_ecoSysInMap(key);
                tmpEcoSysInMapPtr->init();
            }
        }
    }

    //------------------------------//
    // 单为 目标section 绑定 4个端点的 ecosys 实例指针
    //------------------------------//
    sectionPtr->bind_ecoSysInMapPtrs();

    //------------------------------//
    // 为 9个 section 中的所有 chunks （9*4*4） 分配  ecoSysInMap 
    // 这些数据 暂时存储在 section 中
    //------------------------------//
    for( const auto &sPtr : all_nineSectionPtrs.at(_sectionKey) ){ //- each section in nearby-9
        sPtr->assign_chunks_to_ecoSysInMap();
    }

    //------------------------------//
    //  目标section 相关的 4个 ecosysInMap 已经有足够数据，
    //  这 4个 ecosysInMap 开始做 生态规划
    //------------------------------//
    for( auto &ecoSysPtr : sectionPtr->ecoSysInMapPtrs ){ //- each ecosysinmap
        ecoSysPtr->plan();
    }

    //------------------------------//
    // 单为 目标section 生成所有 landWaterEnts 数据
    // 这些数据以 chunk 为单位，暂时存储在 全局容器中
    //------------------------------//
    build_landWaterEnts( _sectionKey );

}


/* ===========================================================
 *                   sec_chunkFieldSet
 * -----------------------------------------------------------
 * 第二阶段
 * 所有操作 都要 ”先检查后实施“。因为有些 实例已经 建设过了
 * -------
 * param: _chunkKey -- 玩家chunk 周边9个 chunk 之一
 */
void sec_chunkFieldSet( chunkKey_t _chunkKey ){

    IntVec2      chunkMPos_ = chunkKey_2_mpos( _chunkKey );
    sectionKey_t sectionKey_ = anyMPos_2_sectionKey( chunkMPos_ );

    //------------------------------//
    // 检查 目标chunk 周边 9个section的 所有 chunk (9*4*4) 的 ChunkFieldSet 实例
    // 若没有，创建之
    // 并 初始化 chunkFieldSet.fields 的一阶数据
    //------------------------------//
    IntVec2         tmpChunkMPos;
    chunkKey_t      tmpChunkKey;
    ChunkFieldSet  *tmpChunkFieldSetPtr;
    for( const auto &sPtr : all_nineSectionPtrs.at(sectionKey_) ){ //- each section in nearby-9

        const IntVec2 &sectionMPosRef = sPtr->get_mpos();

        for( int h=0; h<CHUNKS_PER_SECTION; h++ ){
            for( int w=0; w<CHUNKS_PER_SECTION; w++ ){ //- each chunk in section
                tmpChunkMPos.set(   sectionMPosRef.x + w*ENTS_PER_CHUNK,
                                    sectionMPosRef.y + h*ENTS_PER_CHUNK );
                tmpChunkKey = anyMPos_2_chunkKey( tmpChunkMPos );
                //-----
                if( esrc::chunkFieldSets.find(tmpChunkKey) == esrc::chunkFieldSets.end() ){
                    tmpChunkFieldSetPtr = esrc::insert_new_chunkFieldSet( tmpChunkMPos ); 
                    tmpChunkFieldSetPtr->init_fields();
                }
            }
        }
    }
    
    //------------------------------//
    // 遍历 目标chunk 周边9个 chunkFieldSet 的 所有 field
    // 初始化 field 的二阶数据
    //------------------------------//
    for( auto &chunkFieldSetPtr : get_nearby_chunkFieldSetPtrs(chunkMPos_) ){ //- each chunkFieldSetPtr
        for( auto &fieldPair : chunkFieldSetPtr->fields ){ //- each field
            fieldPair.second.init_secondOrderData();
        }
    }

    //------------------------------//
    //  为 目标chunk 的所有 field 做 cell-noise 分配
    //  在当前测试版中，还为这些 field 分配了颜色
    //------------------------------//
    esrc::get_chunkFieldSetPtr(_chunkKey)->assign_fields_2_chunk();

    esrc::get_chunkFieldSetPtr(_chunkKey)->assign_alti_in_field();

}


/* ===========================================================
 *                get_nearby_chunkFieldSetPtrs
 * -----------------------------------------------------------
 * - 收集 周边 9个 chunkFieldSet 实例指针，并返回容器访问权
 */
std::vector<ChunkFieldSet*> &get_nearby_chunkFieldSetPtrs( const IntVec2 &_anyMPos ){

    nearby_chunkFieldSetPtrs.clear();
    //--------
    IntVec2        currentChunkMPos = anyMPos_2_chunkMPos( _anyMPos );
    IntVec2        nearbyChunkMPos;
    ChunkFieldSet *tmpChunkFieldSetPtr;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            nearbyChunkMPos.set(currentChunkMPos.x + w*ENTS_PER_CHUNK,
                                currentChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkFieldSetPtr = esrc::get_chunkFieldSetPtr( chunkMPos_2_key(nearbyChunkMPos) );
            nearby_chunkFieldSetPtrs.push_back( tmpChunkFieldSetPtr ); //- copy
        }
    }
    //------
    return nearby_chunkFieldSetPtrs;
}



/* ===========================================================
 *              build_landWaterPrefabDatas_for_each_section
 * -----------------------------------------------------------
 * - 检查目标 section 的4个corner 和 8个 edge节点，如果没有为其分配了 landWater 预制件id
 *   分配之
 */
void build_landWaterPrefabDatas_for_each_section( const IntVec2 &_sectionMPos ){

    sectionKey_t  tmpSectionKey;
    chunkKey_t    tmpChunkKey;
    landWaterPrefabEdgeId_t   tmpEdgeId;
    landWaterPrefabCornerId_t tmpCornerId;
 
    for( const auto &cornerDataRef : allCornerDatas_in_section ){ //- each corner
        tmpSectionKey = anyMPos_2_sectionKey( _sectionMPos + cornerDataRef.mposOff );
        if( esrc::landWaterPrefabCornerIds.find(tmpSectionKey) == esrc::landWaterPrefabCornerIds.end() ){
            tmpCornerId = apply_a_rand_landWaterPrefabCornerId();
            esrc::landWaterPrefabCornerIds.insert({ tmpSectionKey, tmpCornerId });
        }
    }

    for( const auto &edgeDataRef : allEdgeDatas_in_section ){ //- each edge
        tmpChunkKey = anyMPos_2_chunkKey( _sectionMPos + edgeDataRef.mposOff );
        if( esrc::landWaterPrefabEdgeIds.find(tmpChunkKey) == esrc::landWaterPrefabEdgeIds.end() ){
            tmpEdgeId = apply_a_rand_landWaterPrefabEdgeId( edgeDataRef.is_leftRight );
            esrc::landWaterPrefabEdgeIds.insert({ tmpChunkKey, tmpEdgeId });
        }
    }
}





}//-------------- namespace : end ----------------//
}//----------------- namespace: sectionBuild: end -------------------//

