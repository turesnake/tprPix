/*
 * ========================= mapSection_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   mapSection 内存态
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "config.h"


#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                insert_new_mapSection
 * -----------------------------------------------------------
 * -- 不是一个 完善的 mapSection 生成器。仅能用于 bypass 阶段
 * -- 注意： 返回的 section 是全空的 ！！！
 * ------
 * param: _sectionMCPos -- “推荐”使用 section左下角 mcpos
 */
MapSection *insert_new_mapSection( const MapCoord &_sectionMCPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapSection section {};
    section.set_by_mapEnt_mpos( _sectionMCPos.get_mpos() );
    u64_t key = section.get_key();
    esrc::mapSections.insert({ key, section }); //- copy
    //-----
    return (MapSection*)&(esrc::mapSections.at(key));
}


/* ===========================================================
 *                  get_memMapEnt
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 mapSection, 从 mapSection.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 mapsection 不存在，就要：加载它／创建它
 * ------
 * param: _mcpos -- 任意mapent 的 mcpos
 */
MemMapEnt *get_memMapEnt( const MapCoord &_mcpos ){

    //-- 计算 目标 mapSection 的 key --
    const IntVec2 &mposRef = _mcpos.get_mpos();
    SectionKey  sectionKey {};
    sectionKey.init_by_mapEnt_mpos( mposRef );

    //-- 拿着key，到 全局容器 esrc::mapSections 中去找。--
    //-- 如果没有，加载／创建它。
        assert( esrc::mapSections.find(sectionKey.get_key()) != esrc::mapSections.end() ); //- tmp
    MapSection &sectionRef = esrc::mapSections.at(sectionKey.get_key());

    //-- 获得 目标 mapEnt 在 section内部的 相对mpos
    IntVec2  lMPosOff = get_section_lMPosOff(mposRef);
    return sectionRef.get_memMapEnt_by_lMPosOff( lMPosOff );
}




}//---------------------- namespace: esrc -------------------------//

