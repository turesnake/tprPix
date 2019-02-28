/*
 * ========================= section_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section 内存态
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "config.h"
#include "SectionKey.h"

#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


/* ===========================================================
 *                insert_new_section
 * -----------------------------------------------------------
 * -- 不是一个 完善的 section 生成器。仅能用于 bypass 阶段
 * -- 注意： 返回的 section 是全空的 ！！！
 * ------
 * param: _sectionMCPos -- “推荐”使用 section左下角 mcpos
 */
Section *insert_new_section( const MapCoord &_sectionMCPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    Section section {};
    section.set_by_mapEnt_mpos( _sectionMCPos.get_mpos() );
    u64_t key = section.get_key();
    esrc::sections.insert({ key, section }); //- copy

        //cout << "sections.size() = " << esrc::sections.size() << endl;
    //-----
    return (Section*)&(esrc::sections.at(key));
}


/* ===========================================================
 *                  get_memMapEnt
 * -----------------------------------------------------------
 * -- 根据参数 _mcpos, 找到其所在的 section, 从 section.memMapEnts
 * -- 找到对应的 mapEnt, 将其指针返回出去
 * -- 如果 目标 section 不存在，就要：加载它／创建它
 * ------
 * param: _mcpos -- 任意mapent 的 mcpos
 */
MemMapEnt *get_memMapEnt( const MapCoord &_mcpos ){

    //-- 计算 目标 section 的 key --
    const IntVec2 &mposRef = _mcpos.get_mpos();
    SectionKey  sectionKey {};
    sectionKey.init_by_mapEnt_mpos( mposRef );

    //-- 拿着key，到 全局容器 esrc::sections 中去找。--
        assert( esrc::sections.find(sectionKey.get_key()) != esrc::sections.end() ); //- tmp
    Section &sectionRef = esrc::sections.at(sectionKey.get_key());

    //-- 获得 目标 mapEnt 在 section内部的 相对mpos
    IntVec2  lMPosOff = get_section_lMPosOff(mposRef);
    return sectionRef.get_memMapEnt_by_lMPosOff( lMPosOff );
}


/* ===========================================================
 *              insert_new_sectionFieldSet
 * -----------------------------------------------------------
 * param: _sectionMCPos -- 必须是 section左下角 mcpos
 */
SectionFieldSet *insert_new_sectionFieldSet( u64_t _sectionKeyVal ){

    // ***| INSERT FIRST, INIT LATER  |***
    MapCoord sectionMCPos = sectionKey_2_mcpos(_sectionKeyVal);
    SectionFieldSet fieldSet {};
    fieldSet.init( sectionMCPos.get_mpos() );
    esrc::sectionFieldSets.insert({ _sectionKeyVal, fieldSet }); //- copy
    //-----
    return (SectionFieldSet*)&(esrc::sectionFieldSets.at(_sectionKeyVal));
}



}//---------------------- namespace: esrc -------------------------//

