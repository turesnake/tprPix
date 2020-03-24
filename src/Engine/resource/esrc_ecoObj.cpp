/*
 * ========================= esrc_ecoObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   ecoObj 内存态
 * ----------------------------
 */
#include "pch.h"
//-------------------- CPP --------------------//
#include <shared_mutex> //- c++17 读写锁

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "GameObj.h"

#include "esrc_ecoObj.h"
#include "esrc_state.h"
#include "esrc_player.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace ecoObj_inn {//------------ namespace: ecoObj_inn --------------//

    //-- ecoObjUPtr 由 job线程 直接创建，move 到本容器中 --
    std::unordered_map<sectionKey_t, std::unique_ptr<EcoObj>> ecoObjs {};
    //std::shared_mutex  sharedMutex; //- 读写锁
                    // 暂时放弃对 ecoObj 数据枷锁
                    // 通过 增大释放圈来避免冲突
                    // 一种非常危险的 策略 
                    // ...

    //--
    size_t delCount {0};

    //-- 仅用于 文件内 各种 atom 函数内 [lock状态] --
    bool is_find_in_ecoObjs_( sectionKey_t key_ ){
        return (ecoObjs.find(key_) != ecoObjs.end());
    }

}//---------------- namespace: ecoObj_inn end --------------//


void init_ecoObjs()noexcept{
    ecoObj_inn::ecoObjs.reserve(10000);
    esrc::insertState("ecoObj");
}



void moveIn_ecoObjUPtr_from_job( sectionKey_t ecoObjKey_, std::unique_ptr<EcoObj> ecoObjUPtr_ ){
    auto [insertIt, insertBool] = ecoObj_inn::ecoObjs.emplace( ecoObjKey_, std::move(ecoObjUPtr_) ); // move
    tprAssert( insertBool );
}


/* 将过于远离 player 的 ecoObj 释放
 */
void del_ecoObjs_tooFarAway()noexcept{

    //--- 并不是每次调用都执行检测和删除 
    //  而是每隔 n 秒执行一次
    if( ecoObj_inn::delCount != 0 ){
        ecoObj_inn::delCount--;
        return;
    }
    ecoObj_inn::delCount = 12 * 10; // 每隔 10 秒一次

    //--------------
    std::set<sectionKey_t> delKeys {};

    int     limit  { 4 * ENTS_PER_SECTION<> };// release-zone 半径 
                                            // 确保这个 班级足够大，大到我们可以不用为 ecoobj 数据加锁
                                            // 一种非常危险的 操作

    IntVec2 playerMPos = dpos_2_mpos(esrc::get_player().get_goRef().get_dpos());
    IntVec2 offMPos {};
    for( const auto &[iKey, iUPtr] : ecoObj_inn::ecoObjs ){
        offMPos = sectionKey_2_mpos(iKey) - playerMPos;
        if( (std::abs(offMPos.x)>limit) || (std::abs(offMPos.y)>limit) ){
            move_ecoObjKey_from_active_2_onReleasing( iKey ); // MUST
            delKeys.insert( iKey );
        }
    }

        tprDebug::console( 
            "ecoObjs.size = {0};      delKeys.size = {1}",
            ecoObj_inn::ecoObjs.size(), delKeys.size()
        );
        
    //---
    size_t eraseNum {};
    for( const auto &key : delKeys ){
        erase_ecoObjKey_from_onReleasing( key ); // MUST
        eraseNum = ecoObj_inn::ecoObjs.erase( key );
        tprAssert( eraseNum == 1 );
    }
}


/* 更加精细的 元素数据 只读访问 接口 [值传递]
 * 仅用于 field 实例 创建阶段
 */
std::unique_ptr<EcoObj_ReadOnly> get_ecoObj_readOnly( sectionKey_t sectionkey_ )noexcept{

    auto readOnlyUPtr = std::make_unique<EcoObj_ReadOnly>();

            tprAssert( ecoObj_inn::is_find_in_ecoObjs_(sectionkey_) );//- must exist
    const auto &ecoObjRef = *(ecoObj_inn::ecoObjs.at(sectionkey_));
                            //-- 切记设置为 负数。
    readOnlyUPtr->sectionKey = ecoObjRef.get_sectionKey();
    readOnlyUPtr->colorTableId = ecoObjRef.get_colorTableId();
    readOnlyUPtr->densitySeaLvlOff = ecoObjRef.get_densitySeaLvlOff();
    readOnlyUPtr->densityDivideValsPtr = ecoObjRef.get_densityDivideValsPtr();
    readOnlyUPtr->ecoObjBorderPtr = ecoObjRef.get_ecoObjBorderPtr();
        //...
                    //-- 目前这个 原子范围 耗时有点长...
    return readOnlyUPtr;
}


EcoObj &get_ecoObjRef( sectionKey_t sectionkey_ )noexcept{
    tprAssert( get_ecoObjMemState(sectionkey_) == EcoObjMemState::Active );
    tprAssert( ecoObj_inn::is_find_in_ecoObjs_(sectionkey_) );//- must exist
    return *(ecoObj_inn::ecoObjs.at(sectionkey_));
}




}//---------------------- namespace: esrc -------------------------//

