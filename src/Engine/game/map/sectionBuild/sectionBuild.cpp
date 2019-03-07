/*
 * ============ sectionBuild.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   存放 section生成器的 通用工具
 * ----------------------------
 */
#include "sectionBuild.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "srcs_engine.h"
#include "chunkKey.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 只在 游戏启动阶段 执行。
 */
void init(){
    //entSideLen_ = Chunk::entSideLen;
    //pixSideLen_ = Chunk::pixSideLen;
    //--------------------------//
    //     初始化  randWH
    //--------------------------//
    randWH.clear();
    randWH.reserve( ENTS_PER_CHUNK * ENTS_PER_CHUNK );
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            randWH.push_back(  IntVec2{ w, h } );
        }
    }
    //...

    //--------------------------//
    //     初始化  randEngine
    //--------------------------//
    // 完成态时，seed应该取自 GameSeed.
    // 目前暂时用 随机seed 替代 tmp
    randEngine.seed( get_new_seed() );

}



/* ===========================================================
 *                     build_data
 * -----------------------------------------------------------
 * -- section 生成器 主函数
 * 生成器主要处理的两个对象：
 *  -- chunkPtr->memMapEnts （先逐个生成数据）
 *  -- chunkPtr->mapTex     （后一股脑制作）
 */
/*
void build_data( Chunk *_chunkPtr ){
    chunkPtr    = _chunkPtr;
    mapTexPtr     = (MapTexture*)&(chunkPtr->mapTex);
    memMapEntsPtr = (std::vector<MemMapEnt>*)&(chunkPtr->memMapEnts);
    //--------

    size_t   pixIdx;
    size_t   entIdx;
    //---

    mapTexPtr->resize_texBuf();
    pixBufHeadPtr = mapTexPtr->get_texBufHeadPtr(); 

    //--------------------------//
    //   初始化 mapEnts
    //--------------------------//
    mapEnts.resize( ENTS_PER_CHUNK * ENTS_PER_CHUNK );
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){ //-- each mapent
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            entIdx = h*ENTS_PER_CHUNK + w;
            mapEnts.at(entIdx).mpos.set( w, h );
        }
    }

    //--------------------------//
    //   初始化  nearbyChunkKeys
    //--------------------------//
    NearbyChunkKey  nsKey;
    //---
    nearbyChunkKeys.clear();
    for( const auto &ikey : chunkPtr->get_near_9_chunkKeys() ){
        nsKey.key = ikey;
        (esrc::chunks.find(ikey)==esrc::chunks.end()) ?
                nsKey.isExisted = true :
                nsKey.isExisted = false;
        nearbyChunkKeys.push_back( nsKey ); //- copy
    }

    //---------------------------//
    //  检测并生成 周边9chunk 的 field信息
    //  esrc::chunkFieldSets
    //---------------------------//
    for( const auto &nbkey : nearbyChunkKeys ){
        if( esrc::chunkFieldSets.find(nbkey.key) == esrc::chunkFieldSets.end() ){
            esrc::insert_new_chunkFieldSet( nbkey.key );
        }
    }

    //---------------------------//
    //  生成本 ChunkFieldSet 的所有 MapField.nearby_nodePPoses
    //---------------------------//
        assert( esrc::chunkFieldSets.find(chunkPtr->get_key()) != esrc::chunkFieldSets.end() );//- tmp
    chunkPtr->fieldSetPtr = (ChunkFieldSet*)&(esrc::chunkFieldSets.at(chunkPtr->get_key()));
    //---
    chunkFieldSetInBuild.init( chunkPtr->fieldSetPtr );


    //---------------------------//
    //  为每一个 mapent，生成 alti
    //---------------------------//


    
    //---------------------------//
    //  为每一个 mapent，划分 ecoSysType
    //  (确保晚于 alti 生成)
    //---------------------------//
    //...


    //---------------------------//
    //  为每一个 mapent，生成 Land-waters
    //  (确保晚于 生态群落)，因为不同的生态群落
    //  其水域生成方式不同
    //---------------------------//
    build_landOrWaters();

    //------


    

    //---------------------------//
    //  遍历每一个像素 1280 * 1280 
    //    生成一张完整的 mapTex 
    //    (一下部分 将被转移)
    //---------------------------//
    //...

    RGBA *pixPtr; //- texBuf 中每个像素点RGBA 的指针。

    IntVec2  pixPPos; //- 像素所在 ppos
    IntVec2  pixMPos; //- 像素所在 entMPos 
    u8_t singleColor;
    //---
    for( int h=0; h<PIXES_PER_CHUNK; h++ ){ //-- each pixel in texure
        for( int w=0; w<PIXES_PER_CHUNK; w++ ){
            
            pixPPos.set( w, h );
            pixMPos = floorDiv( pixPPos, PIXES_PER_MAPENT );
            pixIdx = h*PIXES_PER_CHUNK + w; 
            entIdx = pixMPos.y*ENTS_PER_CHUNK + pixMPos.x;
            //---
            (mapEnts.at(entIdx).isLand==LAND) ?
                singleColor = 160 :
                singleColor = 50;
            
            //mapTex
            pixPtr = pixBufHeadPtr + pixIdx;
            pixPtr->set( singleColor,
                         singleColor,
                         singleColor,
                         255 );
        
        }
    }

    //-- tmp 显示 field 距离场点 [成功] ---
    
    for( const auto &fieldPair : chunkPtr->fieldSetPtr->fields ){
        const MapField &fieldRef = fieldPair.second;
        pixIdx = fieldRef.lNodePPosOff.y*Chunk::pixSideLen + fieldRef.lNodePPosOff.x;
        pixPtr = pixBufHeadPtr + pixIdx;
        pixPtr->set( 255, 0, 0, 255 ); //- red
    }


    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    mapTexPtr->creat_texName();

}
*/



}//----------------- namespace: sectionBuild: end -------------------//

