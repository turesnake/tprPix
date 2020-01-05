/*
 * ==================== GroundRenderPool.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.28
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_GROUND_RENDER_POOL_H
#define TPR_GROUND_RENDER_POOL_H


//-------------------- CPP --------------------//
#include <vector>
#include <map>
#include <unordered_set>

//-------------------- Engine --------------------//
#include "ChildMesh.h"
#include "Mesh.h"
#include "colorTableId.h"


class GroundRenderPool{
public:
    GroundRenderPool()
        {
            this->init();
        }

    void init()noexcept;
    
    inline void insert( colorTableId_t id_, float zOff_, ChildMesh *meshPtr_ )noexcept{
        auto target = this->pools.find(id_);
        tprAssert( target != this->pools.end() );
        target->second.insert({ zOff_, meshPtr_ });// multi
    }
    

    inline void clear()noexcept{ 
        for( auto &pair : this->pools ){
            pair.second.clear();
        }
    }

    void draw()noexcept;

private:

    std::unordered_map<colorTableId_t, std::multimap<float, ChildMesh*>> pools {};
};





#endif 

