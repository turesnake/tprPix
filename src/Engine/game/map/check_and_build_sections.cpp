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
#include "Section.h"
#include "srcs_engine.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "sectionBuild.h"

#include "debug.h"

namespace{//----------- namespace ----------------//

    u64_t   lastSectionKey    {}; //- 上次检测时，记录的 玩家所在 section.key
    u64_t   currentSectionKey {}; //- 此次检测时，玩家所在 section.key

    void create_a_section( const MapCoord &_sectionMCPos );

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

    //.. 二话不说直接创建 目标section
    //   调用本函数，说明一定处于 “无视存储” 的早期阶段。
    MapCoord sectionMCPos { get_section_mpos(_mpos) };
    create_a_section( sectionMCPos );
}


/* ===========================================================
 *                   build_first_section
 * -----------------------------------------------------------
 * -- 创建目标 section 周边8个 section
 */
void build_nearby_sections( const IntVec2 &_mpos ){

    //.. 二话不说直接创建 目标section 周围8个 section
    //   调用本函数，说明一定处于 “无视存储” 的早期阶段。
    u64_t key = just_creat_sectionKey_by_mpos( _mpos );

        assert( esrc::sections.find(key) != esrc::sections.end() ); //- tmp

    const auto &keys = esrc::sections.at(key).get_near_9_sectionKeys();
    for( const auto &ikey : keys ){
        if( esrc::sections.find(ikey) == esrc::sections.end() ){
            //-- 创建 这个 section
            create_a_section( sectionKey_2_mcpos(ikey) );
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

    //-- 没有变更section时，直接终止检测 ---
    currentSectionKey = just_creat_sectionKey_by_mpos( playerMPos );
    if( currentSectionKey == lastSectionKey ){
        return;
    }
    lastSectionKey = currentSectionKey;

    //-------------------------//
    // 检测并 创建 周边 8 个 section
    // 默认 自身所处section 已经创建了
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
 * param: _sectionMCPos -- section 左下角mapent mcpos （强制）
 */
void create_a_section( const MapCoord &_sectionMCPos ){

    Section *sectionPtr = esrc::insert_new_section( _sectionMCPos );

    //---- 填充满 mapSection.memMapEnts ----
    for( int h=0; h<SECTION_SIDE_ENTS; h++ ){
        for( int w=0; w<SECTION_SIDE_ENTS; w++ ){
            MemMapEnt mapEnt {};
            mapEnt.mcpos = sectionPtr->get_mcpos() + MapCoord{ w, h };
            sectionPtr->memMapEnts.push_back( mapEnt ); //-copy
        }
    }

    //----- 正式生成 section 数据 -----//
    //sectionPtr->build_new_section(); //- 此时目标 section 是空的
    sectionBuild::build( sectionPtr ); //- 此时目标 section 是空的

    //-- bind mapTex - mesh --
    sectionPtr->mesh.init( sectionPtr->mapTex.get_texName() );
    sectionPtr->mesh.set_shader_program( &(esrc::rect_shader) );
    sectionPtr->mesh.isVisible = true;

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    sectionPtr->refresh_translate_auto();
    sectionPtr->init();

}

}//-------------- namespace : end ----------------//

