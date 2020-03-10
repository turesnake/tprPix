/*
 * ================== GroundRenderPool.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GroundRenderPool.h"


//-------------------- Engine --------------------//
#include "esrc_colorTableSet.h"

#include "ubo_all.h"


//-- Must after esrc::init_colorTableSet !!!
void GroundRenderPool::init()noexcept{

    auto &colorTableSet = esrc::get_colorTabelSet();
    for( const auto &[iName, iId] : colorTableSet.get_name_idsRef() ){
        if( iName == "origin" ){ // skip
           continue; 
        }
        auto [insertIt, insertBool] = this->pools.emplace( iId, std::multimap<float, ChildMesh*>{} );
        tprAssert( insertBool );
    }
}


void GroundRenderPool::draw()noexcept{

    for( const auto &[iId, iMMap] : this->pools ){

        if( iMMap.empty() ){
            continue; // skip
        }

        //-- 每次渲染前，要先改写 uniform 值
        ubo::write_ubo_colorTableId( iId );

        //- 先渲染 pos.z 值大的（近处优先）
        for( auto it = iMMap.rbegin(); 
            it != iMMap.rend(); it++  ){
            it->second->draw();
        }
    }
}

