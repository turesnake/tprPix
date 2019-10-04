/*
 * ========================= esrc_ecoObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ecoObj 内存态
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <shared_mutex> //- c++17 读写锁
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "sectionKey.h"

#include "esrc_ecoObj.h"
#include "esrc_state.h"

//#include "tprDebug.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace ecoObj_inn {//------------ namespace: ecoObj_inn --------------//

    //-- 目前版本中，只有 主线程可以 创建 ecoObj 实例 --
    //   仅提供 数据只读口给 job线程 
    std::unordered_map<sectionKey_t, std::unique_ptr<EcoObj>> ecoObjs {};
    std::shared_mutex  sharedMutex; //- 读写锁

    //-- 仅用于 文件内 各种 atom 函数内 [lock状态] --
    bool is_find_in_ecoObjs_( sectionKey_t key_ ){
        return (ecoObjs.find(key_) != ecoObjs.end());
    }

}//---------------- namespace: ecoObj_inn end --------------//


void init_ecoObjs()noexcept{
    ecoObj_inn::ecoObjs.reserve(10000);
    esrc::insertState("ecoObj");
}

/* ===========================================================
 *               atom_insert_new_ecoObj     [-WRITE-]
 * -----------------------------------------------------------
 * 一步到位地实现 任意 ecoObj 实例 的创建
 * 此函数 只应在 主线程（唯一写入者） 中被执行 
 */
void atom_try_to_inert_and_init_a_ecoObj( sectionKey_t ecoObjKey_ )noexcept{

    //=== lock ===//
    std::unique_lock<std::shared_mutex> ul( ecoObj_inn::sharedMutex ); //- write -
    //-- 若已存在，直接返回 --
    if( ecoObj_inn::is_find_in_ecoObjs_(ecoObjKey_) ){
        return;
    }

        //=== unlock ===//
        ul.unlock();
        //-- 计算出周边 4个 node key --
        std::vector<sectionKey_t> nearby_four_nodeKeys {};
        EcoObj::calc_nearFour_node_ecoObjKey( ecoObjKey_, nearby_four_nodeKeys );

        std::vector<sectionKey_t> nearby_four_ecoSysIds {}; //- 需要传给 init_for_regular()

    //=== lock ===//
    ul.lock();
    for( const auto &key : nearby_four_nodeKeys ){
        //-- 已经存在的 node 实例 不需要创建 --
        if( ecoObj_inn::is_find_in_ecoObjs_( key ) ){
            nearby_four_ecoSysIds.push_back( ecoObj_inn::ecoObjs.at(key)->get_ecoSysPlanId() ); //- copy
            continue;
        }

            //=== unlock ===//
            ul.unlock();
            auto ecoObjUPtr = std::make_unique<EcoObj>();
            ecoObjUPtr->init_for_node( key );
            nearby_four_ecoSysIds.push_back( ecoObjUPtr->get_ecoSysPlanId() ); //- copy

        //=== lock ===//
        ul.lock();
        ecoObj_inn::ecoObjs.insert({ key, std::move(ecoObjUPtr) }); //- copy
    }
    
    //=== still locked ===//
    tprAssert( nearby_four_ecoSysIds.size() == 4 );
    //-- 若 目标 ecoObj 实例 就是 node
    //   则此时会直接返回
    if( ecoObj_inn::is_find_in_ecoObjs_(ecoObjKey_) ){
        return;
    }

        //=== unlock ===//
        ul.unlock();
        auto targetEntUPtr = std::make_unique<EcoObj>();
        targetEntUPtr->init_for_regular( ecoObjKey_, nearby_four_ecoSysIds );

    //=== lock ===//
    ul.lock();
    ecoObj_inn::ecoObjs.insert({ ecoObjKey_, std::move(targetEntUPtr) }); //- copy
}


/* ===========================================================
 *          atom_get_ecoObj_sectionKey      [-READ-]
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口 [值传递]
 * 仅用于 field 实例 创建阶段
 */
std::pair<occupyWeight_t, EcoObj_ReadOnly> atom_get_ecoObj_readOnly( sectionKey_t sectionkey_ )noexcept{

    std::pair<occupyWeight_t, EcoObj_ReadOnly>  readOnly {};
    {//--- atom ---//
        std::shared_lock<std::shared_mutex> sl( ecoObj_inn::sharedMutex ); //- read -
            tprAssert( ecoObj_inn::is_find_in_ecoObjs_(sectionkey_) );//- must exist
        const auto &ecoObjRef = *(ecoObj_inn::ecoObjs.at(sectionkey_));
        readOnly.first = -ecoObjRef.get_occupyWeight();
                            //-- 切记设置为 负数。
        readOnly.second.sectionKey = ecoObjRef.get_sectionKey();
        readOnly.second.colorTableId = ecoObjRef.get_colorTableId();
        readOnly.second.densitySeaLvlOff = ecoObjRef.get_densitySeaLvlOff();
        readOnly.second.densityDivideValsPtr = ecoObjRef.get_densityDivideValsPtr();
        //...
                    //-- 目前这个 原子范围 耗时有点长...
    }
    return readOnly;
}



/* ===========================================================
 *          atom_ecoObj_apply_a_rand_goSpecData    [-READ-]
 * -----------------------------------------------------------
 */
const GoSpecData &atom_ecoObj_apply_a_rand_goSpecData(sectionKey_t sectionkey_, 
                                                    size_t densityIdx_, 
                                                    double randV_ )noexcept{
    //--- atom ---//
    std::shared_lock<std::shared_mutex> sl( ecoObj_inn::sharedMutex ); //- read -
        tprAssert( ecoObj_inn::is_find_in_ecoObjs_(sectionkey_) );//- must exist
    return ecoObj_inn::ecoObjs.at(sectionkey_)->apply_a_rand_goSpecData( densityIdx_, randV_  );
}

/* ===========================================================
 *          atom_ecoObj_get_applyPercent     [-READ-]
 * -----------------------------------------------------------
 * -- 更加精细的 元素数据 只读访问 接口
 */
double atom_ecoObj_get_applyPercent( sectionKey_t sectionkey_, Density density_ )noexcept{
    //--- atom ---//
    std::shared_lock<std::shared_mutex> sl( ecoObj_inn::sharedMutex ); //- read -
        tprAssert( ecoObj_inn::is_find_in_ecoObjs_(sectionkey_) );//- must exist
    return ecoObj_inn::ecoObjs.at(sectionkey_)->get_applyPercent( density_ );
}


colorTableId_t atom_ecoObj_get_colorTableId( sectionKey_t sectionkey_ )noexcept{
    //--- atom ---//
    std::shared_lock<std::shared_mutex> sl( ecoObj_inn::sharedMutex ); //- read -
        tprAssert( ecoObj_inn::is_find_in_ecoObjs_(sectionkey_) );//- must exist
    return ecoObj_inn::ecoObjs.at(sectionkey_)->get_colorTableId();
}






}//---------------------- namespace: esrc -------------------------//

