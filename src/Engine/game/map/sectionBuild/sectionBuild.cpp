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
#include "SectionKey.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 只在 游戏启动阶段 执行。
 */
void init(){
    entSideLen_ = Section::entSideLen;
    pixSideLen_ = Section::pixSideLen;
    //--------------------------//
    //     初始化  randWH
    //--------------------------//
    randWH.clear();
    randWH.reserve( entSideLen_ * entSideLen_ );
    for( int h=0; h<entSideLen_; h++ ){
        for( int w=0; w<entSideLen_; w++ ){
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
 *                     build
 * -----------------------------------------------------------
 * -- section 生成器 主函数
 * 生成器主要处理的两个对象：
 *  -- sectionPtr->memMapEnts （先逐个生成数据）
 *  -- sectionPtr->mapTex     （后一股脑制作）
 */
void build( Section *_sectionPtr ){
    sectionPtr    = _sectionPtr;
    mapTexPtr     = (MapTexture*)&(sectionPtr->mapTex);
    memMapEntsPtr = (std::vector<MemMapEnt>*)&(sectionPtr->memMapEnts);
    //--------

    size_t   pixIdx;
    size_t   entIdx;
    //---

    mapTexPtr->resize_texBuf();
    pixBufHeadPtr = mapTexPtr->get_texBufHeadPtr(); 

    //--------------------------//
    //   初始化 mapEnts
    //--------------------------//
    mapEnts.resize( entSideLen_ * entSideLen_ );
    for( int h=0; h<entSideLen_; h++ ){ //-- each mapent
        for( int w=0; w<entSideLen_; w++ ){
            entIdx = h*entSideLen_ + w;
            mapEnts.at(entIdx).mpos.set( w, h );
        }
    }

    //--------------------------//
    //   初始化  nearbySectionKeys
    //--------------------------//
    NearbySectionKey  nsKey;
    //---
    nearbySectionKeys.clear();
    for( const auto &ikey : sectionPtr->get_near_9_sectionKeys() ){
        nsKey.key = ikey;
        (esrc::sections.find(ikey)==esrc::sections.end()) ?
                nsKey.isExisted = true :
                nsKey.isExisted = false;
        nearbySectionKeys.push_back( nsKey ); //- copy
    }

    //---------------------------//
    //  检测并生成 周边9section 的 field信息
    //  esrc::sectionFieldSets
    //---------------------------//
    for( const auto &nbkey : nearbySectionKeys ){
        if( esrc::sectionFieldSets.find(nbkey.key) == esrc::sectionFieldSets.end() ){
            esrc::insert_new_sectionFieldSet( nbkey.key );
        }
    }

    //---------------------------//
    //  生成本SectionFieldSet 的所有 MapField.nearby_nodePPoses
    //---------------------------//
        assert( esrc::sectionFieldSets.find(sectionPtr->get_key()) != esrc::sectionFieldSets.end() );//- tmp
    sectionPtr->fieldSetPtr = (SectionFieldSet*)&(esrc::sectionFieldSets.at(sectionPtr->get_key()));
    //---
    /*
    for( auto &fieldRef : fieldSetPtr->fields ){
        fieldRef.build_nearby_nodePPoses();
    }
    */
    sectionFieldSetInBuild.init( sectionPtr->fieldSetPtr );


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


    /*
    for( int h=0; h<entSideLen; h++ ){
        for( int w=0; w<entSideLen; w++ ){
        }
    }
    */

    //---------------------------//
    //  遍历每一个像素 1280 * 1280 
    //    生成一张完整的 mapTex 
    //---------------------------//
    //...

    RGBA *pixPtr; //- texBuf 中每个像素点RGBA 的指针。

    IntVec2  pixPPos; //- 像素所在 ppos
    IntVec2  pixMPos; //- 像素所在 entMPos 
    u8_t singleColor;
    //---
    for( int h=0; h<pixSideLen_; h++ ){ //-- each pixel in texure
        for( int w=0; w<pixSideLen_; w++ ){
            
            pixPPos.set( w, h );
            pixMPos = floorDiv( pixPPos, PIXES_PER_MAPENT );
            pixIdx = h*pixSideLen_ + w; 
            entIdx = pixMPos.y*entSideLen_ + pixMPos.x;
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
    
    for( const auto &fieldRef : sectionPtr->fieldSetPtr->fields ){
        pixIdx = fieldRef.lNodePPosOff.y*Section::pixSideLen + fieldRef.lNodePPosOff.x;
        pixPtr = pixBufHeadPtr + pixIdx;
        pixPtr->set( 255, 0, 0, 255 ); //- red
    }


    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    mapTexPtr->creat_texName();

}


}//----------------- namespace: sectionBuild: end -------------------//

