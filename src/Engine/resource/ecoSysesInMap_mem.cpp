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
#include <shared_mutex> //- c++17 读写锁

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
    std::shared_mutex  sharedMutex; //- 读写锁

    //-- 仅用于 文件内 各种 atom 函数内 [lock状态] --
    bool is_find_in_ecoSysesInMap_( sectionKey_t _key ){
        return (ecoSysesInMap.find(_key) != ecoSysesInMap.end());
    }

}//---------------- namespace end --------------//


/* ===========================================================
 *               atom_insert_new_ecoSysInMap     [-WRITE-]
 * -----------------------------------------------------------
 * 一步到位地实现 任意 ecoSysInMap 实例 的创建
 * 此函数 只应在 主线程（唯一写入者） 中被执行 
 */
void atom_try_to_inert_and_init_a_ecoSysInMap( sectionKey_t _ecoSysInMapKey ){

    //=== lock ===//
    std::unique_lock<std::shared_mutex> ul( sharedMutex ); //- write -
    //-- 若已存在，直接返回 --
    if( is_find_in_ecoSysesInMap_(_ecoSysInMapKey) ){
        return;
    }

        //=== unlock ===//
        ul.unlock();
        //-- 计算出周边 4个 node key --
        std::vector<sectionKey_t> nearby_four_nodeKeys {};
        EcoSysInMap::calc_nearFour_node_ecoSysInMapKey( _ecoSysInMapKey, nearby_four_nodeKeys );

        std::vector<sectionKey_t> nearby_four_ecoSysIds {}; //- 需要传给 init_for_regular()

    //=== lock ===//
    ul.lock();
    for( const auto &key : nearby_four_nodeKeys ){
        //-- 已经存在的 node 实例 不需要创建 --
        if( is_find_in_ecoSysesInMap_( key ) ){
            nearby_four_ecoSysIds.push_back( esrc::ecoSysesInMap.at(key).get_ecoSysId() ); //- copy
            continue;
        }

            //=== unlock ===//
            ul.unlock();
            EcoSysInMap ecoSysInMap {};
            ecoSysInMap.init_for_node( key );
            nearby_four_ecoSysIds.push_back( ecoSysInMap.get_ecoSysId() ); //- copy

        //=== lock ===//
        ul.lock();
        esrc::ecoSysesInMap.insert({ key, ecoSysInMap }); //- copy
    }
    
    //=== still locked ===//
    assert( nearby_four_ecoSysIds.size() == 4 );
    //-- 若 目标 ecoSysInMap 实例 就是 node
    //   则此时会直接返回
    if( is_find_in_ecoSysesInMap_(_ecoSysInMapKey) ){
        return;
    }

        //=== unlock ===//
        ul.unlock();
        EcoSysInMap targetEnt {};
        targetEnt.init_for_regular( _ecoSysInMapKey, nearby_four_ecoSysIds );
        

    //=== lock ===//
    ul.lock();
    esrc::ecoSysesInMap.insert({ _ecoSysInMapKey, targetEnt }); //- copy
}


/* ===========================================================
 *          atom_get_ecoSysInMap_sectionKey      [-READ-]
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口 [值传递]
 * 仅用于 field 实例 创建阶段
 */
const std::pair<occupyWeight_t, EcoSysInMap_ReadOnly> atom_get_ecoSysInMap_readOnly( sectionKey_t _sectionkey ){

    std::pair<occupyWeight_t, EcoSysInMap_ReadOnly>  readOnly;
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( sharedMutex ); //- read -
            assert( is_find_in_ecoSysesInMap_(_sectionkey) );//- must exist
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
 *          atom_get_ecoSysInMap_landColorsPtr     [-READ-]
 * -----------------------------------------------------------
 *    实际数据在 ecosys 实例中（不用担心会不存在）
 *    所以可以在此处，放心地传递 指针
 */
const std::vector<RGBA> *atom_get_ecoSysInMap_landColorsPtr( sectionKey_t _sectionkey ){

    const std::vector<RGBA> *ptr;
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( sharedMutex ); //- read -
            assert( is_find_in_ecoSysesInMap_(_sectionkey) );//- must exist
        ptr = esrc::ecoSysesInMap.at(_sectionkey).get_landColorsPtr();
    }
    return ptr;
}

/* ===========================================================
 *          atom_ecoSysInMap_apply_a_rand_goSpecId    [-READ-]
 * -----------------------------------------------------------
 */
const goSpecId_t atom_ecoSysInMap_apply_a_rand_goSpecId(sectionKey_t _sectionkey, size_t _densityIdx, float _randV ){
    goSpecId_t id;
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( sharedMutex ); //- read -
            assert( is_find_in_ecoSysesInMap_(_sectionkey) );//- must exist
        id = esrc::ecoSysesInMap.at(_sectionkey).apply_a_rand_goSpecId( _densityIdx, _randV  );
    }
    return id;
}

/* ===========================================================
 *          atom_ecoSysInMap_get_applyPercent     [-READ-]
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口
 */
const float atom_ecoSysInMap_get_applyPercent( sectionKey_t _sectionkey, const Density &_density ){
    float val;
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( sharedMutex ); //- read -
            assert( is_find_in_ecoSysesInMap_(_sectionkey) );//- must exist
        val = esrc::ecoSysesInMap.at(_sectionkey).get_applyPercent( _density );
    }
    return val;
}




}//---------------------- namespace: esrc -------------------------//

