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
 * -- 不断 扩充 中。
 * -- 参数 _sectionPos “推荐”使用 section左下角 mcpos
 */
MapSection *insert_new_mapSection( const MapCoord &_sectionPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapSection section {};
    section.set_by_mapEnt_mpos( _sectionPos.get_mpos() );
    u64_t key = section.get_key();
    esrc::mapSections.insert({ key, section }); //- copy
    //-----
    MapSection &sectionRef = esrc::mapSections.at(key);

    //-- 填充满 mapSection.memMapEnts --
    for( int h=0; h<SECTION_H_ENTS; h++ ){
        for( int w=0; w<SECTION_W_ENTS; w++ ){

            MemMapEnt mapEnt {};
            mapEnt.mcpos = sectionRef.get_mcpos() + MapCoord{ w, h };
            //...

            sectionRef.memMapEnts.push_back( mapEnt ); //-copy
        }
    }

    return (MapSection*)&sectionRef;
}


/* ===========================================================
 *                  get_memMapEnt
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 mapSection, 从 mapSection.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 mapsection 不存在，就要：加载它／创建它
 */
MemMapEnt *get_memMapEnt( const MapCoord &_mcpos ){

    //-- 计算 目标 mapSection 的 key --
    const IntVec2 &mposRef = _mcpos.get_mpos();
    SectionKey key;
    key.init_by_mapEnt_mpos( mposRef );

    //-- 拿着key，到 全局容器 esrc::mapSections 中去找。--
    //-- 如果没有，加载／创建它。
    //-- 目前假定只有一个 section。超出的直接报错 
        assert( key.get_key() == 0 );
    MapSection &sectionRef = esrc::mapSections.at(key.get_key());

    //-- 获得 目标 mapEnt 在 section内部的 相对mpos
    IntVec2 lmpos { mposRef.x%SECTION_W_ENTS, 
                    mposRef.y%SECTION_H_ENTS };
    
    //int idx = lmpos.y*SECTION_W_ENTS + lmpos.x;
    //return (MemMapEnt*)&(sectionRef.memMapEnts.at(idx));
    return sectionRef.get_memMapEnt_by_lmpos( lmpos );
}




}//---------------------- namespace: esrc -------------------------//

