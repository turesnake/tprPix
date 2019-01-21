/*
 * ========================= mapSection_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.16
 *                                        修改 -- 2019.01.16
 * ----------------------------------------------------------
 *   mapSection 内存态
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                insert_new_mapSection
 * -----------------------------------------------------------
 * -- 不是一个 完善的 mapSection 生成器。仅能用于 bypass 阶段
 * -- 不断 扩充 中。
 */
MapSection *insert_new_mapSection( const MapCoord &_sectionPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapSection section {};
    section.set_by_ppos( _sectionPos.get_ppos() );
    u64 key = section.get_key();
    esrc::mapSections.insert({ key, section }); //- copy
    return (MapSection*)&(esrc::mapSections.at(key));
}


/* ===========================================================
 *                  get_memMapEnt
 * -----------------------------------------------------------
 * -- 根据参数 _mc, 找到其所在的 mapSection, 从 mapSection.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 mapsection 不存在，就要：加载它／创建它
 */
MemMapEnt *get_memMapEnt( const MapCoord &_mc ){

    //-- 计算 目标 mapSection 的 key --
    const IntVec2 &mposRef = _mc.get_mpos();
    SectionKey key;
    key.init_by_mpos( mposRef );

    //-- 拿着key，到 全局容器 esrc::mapSections 中去找。--
    //-- 如果没有，加载／创建它。
    //-- 目前假定只有一个 section。超出的直接报错 
    assert( key.get_key() == 0 );
    MapSection &sectionRef = esrc::mapSections.at(key.get_key());

    //-- 获得 目标 mapEnt 在 section内部的 相对mpos
    IntVec2 lmpos { mposRef.x%SECTION_W_ENTS, 
                    mposRef.y%SECTION_H_ENTS };
    
    int idx = lmpos.y*SECTION_W_ENTS + lmpos.x;

    return (MemMapEnt*)&(sectionRef.memMapEnts.at(idx));
}




}//---------------------- namespace: esrc -------------------------//

