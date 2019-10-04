/*
 * ======================= RenderPool.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.22
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_RENDER_POOL_H
#define TPR_RENDER_POOL_H


//-------------------- CPP --------------------//
#include <vector>
#include <map>
#include <unordered_set>

//-------------------- Engine --------------------//
#include "ChildMesh.h"
#include "Mesh.h"



enum class RenderPoolType{
    Nil,   
    Opaque,     // above water
    Translucent, // above water
    Shadow // tmp
};



class RenderPool{
public:
    RenderPool( bool isOpaque_ ):
        isOpaque(isOpaque_)
        {}

    inline void insert( float off_z_, ChildMesh *meshPtr_ )noexcept{
        this->pool.insert({ off_z_, meshPtr_ });// multi
    }

    inline void clear()noexcept{ this->pool.clear(); }

    inline void draw()noexcept{
        if( this->isOpaque ){ //- true:  先渲染 pos.z 值大的（近处优先）
            for( auto it = this->pool.rbegin(); 
                it != this->pool.rend(); it++  ){
                    it->second->draw();
            }
        }else{//- false: 先渲染 pos.z 值小的（远处优先）
            for( auto &pair : this->pool ){
                pair.second->draw();
            }
        }
    }

private:
    std::multimap<float, ChildMesh*> pool {};
    bool isOpaque;
            //- true:  先渲染 pos.z 值大的（近处优先）
            //- false: 先渲染 pos.z 值小的（远处优先）
};





#endif 

