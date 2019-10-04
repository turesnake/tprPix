/*
 * ================== GroundRenderPool.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "GroundRenderPool.h"


//-------------------- Engine --------------------//
#include "esrc_colorTableSet.h"

#include "ubo_all.h"

#include "tprDebug.h"

//-- Must after esrc::init_colorTableSet !!!
void GroundRenderPool::init()noexcept{

    auto &colorTableSet = esrc::get_colorTabelSet();
    for( const auto &pair : colorTableSet.get_name_idsRef() ){
        if( pair.first == "origin" ){ // skip
           continue; 
        }
        auto outPair = this->pools.insert({ pair.second, std::multimap<float, ChildMesh*>{} });
        tprAssert( outPair.second );
    }
    cout << endl;
}


void GroundRenderPool::draw()noexcept{

    for( const auto &pair : this->pools ){

        if( pair.second.empty() ){
            continue; // skip
        }

        //-- 每次渲染前，要先改写 uniform 值
        ubo::write_ubo_colorTableId( pair.first );

        //- 先渲染 pos.z 值大的（近处优先）
        for( auto it = pair.second.rbegin(); 
            it != pair.second.rend(); it++  ){
            it->second->draw();
        }
    }
}

