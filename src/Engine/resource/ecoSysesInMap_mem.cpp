/*
 * ========================= ecoSysesInMap_mem.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ecoSysInMap 内存态
 * ----------------------------
 */
//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <mutex>

//-------------------- Engine --------------------//
#include "esrc_ecoSysInMap.h"

#include "config.h"
#include "sectionKey.h"

//#include "debug.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//

namespace{//------------ namespace --------------//

    //-- 目前版本中，只有 主现场可以 创建 ecosysinmap 实例 --
    //   仅提供 数据只读口给 job线程 
    std::unordered_map<sectionKey_t, EcoSysInMap> ecoSysesInMap {};
    std::mutex  ecoSysInMapMutex;


}//---------------- namespace end --------------//


/* ===========================================================
 *               atom_insert_new_ecoSysInMap
 * -----------------------------------------------------------
 * param: _sectionMPos --  seection左下角 mcpos
 * 已初始化数据：
 *  - sectionKey
 *  - mcpos
 */
EcoSysInMap *atom_insert_new_ecoSysInMap( const IntVec2 &_sectionMPos ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSysInMap *ptr;
    EcoSysInMap  ecoSysInMap {};
    ecoSysInMap.set_by_sectionMPos( _sectionMPos );
    sectionKey_t  key = ecoSysInMap.get_sectionKey();

    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(key) == esrc::ecoSysesInMap.end() );//- must not exist
        esrc::ecoSysesInMap.insert({ key, ecoSysInMap }); //- copy
        ptr = &(esrc::ecoSysesInMap.at(key));
    }
    return ptr;
}


/* ===========================================================
 *            atom_find_and_return_ecoSysesInMapPtr
 * -----------------------------------------------------------
 * 将 find + get 两个操作 合并原子化 
 * -- 若没找到，返回 nullptr
 */
EcoSysInMap *atom_find_and_return_ecoSysesInMapPtr( sectionKey_t _sectionKey ){
    EcoSysInMap *ptr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
        (esrc::ecoSysesInMap.find(_sectionKey) != esrc::ecoSysesInMap.end()) ?
                ptr = &(esrc::ecoSysesInMap.at(_sectionKey)) :
                ptr = nullptr;
    }
    return ptr;
}


/* ===========================================================
 *              atom_get_ecoSysInMap_sectionKey
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口 [值传递]
 * 仅用于 field 实例 创建阶段
 */
const std::pair<occupyWeight_t, EcoSysInMap_ReadOnly> atom_get_ecoSysInMap_readOnly( sectionKey_t _sectionkey ){

    std::pair<occupyWeight_t, EcoSysInMap_ReadOnly>  readOnly;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(_sectionkey) != esrc::ecoSysesInMap.end() );//- tmp
        const auto &inMap = esrc::ecoSysesInMap.at(_sectionkey);
        readOnly.first = -inMap.get_occupyWeight();
                            //-- 切记设置为 负数。
        readOnly.second.sectionKey = inMap.get_sectionKey();
        readOnly.second.densitySeaLvlOff = inMap.get_densitySeaLvlOff();
        readOnly.second.densityDivideValsPtr = inMap.get_densityDivideValsPtr();
        //...
                    //-- 目前这个 原子范围 耗时有点长...
    }
    return readOnly;
}


/* ===========================================================
 *              atom_get_ecoSysInMap_sectionKey
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口
 *    实际数据在 ecosys 实例中（不用担心会不存在）
 *    所以可以在此处，放心地传递 指针
 */
const std::vector<RGBA> *atom_get_ecoSysInMap_landColorsPtr( sectionKey_t _sectionkey ){

    const std::vector<RGBA> *ptr;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(_sectionkey) != esrc::ecoSysesInMap.end() );//- tmp
        ptr = esrc::ecoSysesInMap.at(_sectionkey).get_landColorsPtr();
    }
    return ptr;
}

/* ===========================================================
 *             atom_ecoSysInMap_apply_a_rand_goSpecId
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口
 */
const goSpecId_t atom_ecoSysInMap_apply_a_rand_goSpecId(sectionKey_t _sectionkey, size_t _densityIdx, float _randV ){
    goSpecId_t id;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(_sectionkey) != esrc::ecoSysesInMap.end() );//- tmp
        id = esrc::ecoSysesInMap.at(_sectionkey).apply_a_rand_goSpecId( _densityIdx, _randV  );
    }
    return id;
}

/* ===========================================================
 *          atom_ecoSysInMap_get_applyPercent
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口
 */
const float atom_ecoSysInMap_get_applyPercent( sectionKey_t _sectionkey, const Density &_density ){
    float val;
    {//--- atom ---//
        std::lock_guard<std::mutex> lg(ecoSysInMapMutex);
            assert( esrc::ecoSysesInMap.find(_sectionkey) != esrc::ecoSysesInMap.end() );//- tmp
        val = esrc::ecoSysesInMap.at(_sectionkey).get_applyPercent( _density );
    }
    return val;
}










}//---------------------- namespace: esrc -------------------------//

