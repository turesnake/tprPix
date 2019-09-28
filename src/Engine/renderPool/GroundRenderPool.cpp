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


#include "tprDebug.h"

//-- Must after esrc::init_colorTableSet !!!
void GroundRenderPool::init()noexcept{

    auto &colorTableSet = esrc::get_colorTabelSet();
    for( const auto &pair : colorTableSet.get_name_idsRef() ){
        this->pools.insert({ pair.second, std::multimap<float, ChildMesh*>{} });
    }
}



void GroundRenderPool::insert( colorTableId_t id_, float off_z_, ChildMesh *meshPtr_ )noexcept{

        if( this->pools.find(id_) == this->pools.end() ){
            cout << "   colorTableId = " << id_ << endl;
        }

        tprAssert( this->pools.find(id_) != this->pools.end() );
        this->pools.at(id_).insert({ off_z_, meshPtr_ });
}

void GroundRenderPool::draw()noexcept{

    for( const auto &pair : this->pools ){


        //-- 每次渲染前，要先改写 uniform 值



        //- 先渲染 pos.z 值大的（近处优先）
        for( auto it = pair.second.rbegin(); 
            it != pair.second.rend(); it++  ){
            it->second->draw();
        }
    }
}

