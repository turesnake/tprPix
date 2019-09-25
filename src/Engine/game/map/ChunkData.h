/*
 * ========================== ChunkData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#ifndef TPR_CHUNK_DATA_H
#define TPR_CHUNK_DATA_H

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "MapTexture.h"
#include "MapAltitude.h"
#include "fieldKey.h"


class ChunkData{
public:
    ChunkData() = default;

    //-- init --//
    inline void init_mapEntAltis()noexcept{
        this->mapEntAltis.resize( ENTS_PER_CHUNK * ENTS_PER_CHUNK );
    }

    //-- set --//
    inline void set_mapEntAlti( size_t idx_, MapAltitude alti_ )noexcept{
        tprAssert( idx_ < this->mapEntAltis.size() );
        this->mapEntAltis.at(idx_) = alti_;
    }

    //-- get --//    
    inline const std::vector<MapAltitude> &get_mapEntAltis() const noexcept{
        return this->mapEntAltis;
    }


private:
    std::vector<MapAltitude>  mapEntAltis {};
};


#endif 

