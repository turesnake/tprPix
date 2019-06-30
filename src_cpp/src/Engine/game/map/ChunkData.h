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
    inline void resize_texBuf(){
        this->texBuf.resize(  PIXES_PER_CHUNK_IN_TEXTURE * PIXES_PER_CHUNK_IN_TEXTURE, 
                            RGBA{ 0,0,0,0 } );
    }
    inline void init_mapEntAltis(){
        this->mapEntAltis.resize( ENTS_PER_CHUNK * ENTS_PER_CHUNK );
    }

    //-- set --//
    inline void set_mapEntAlti( size_t idx_, const MapAltitude &alti_ ){
        tprAssert( idx_ < this->mapEntAltis.size() );
        this->mapEntAltis.at(idx_) = alti_;
    }

    //-- get --//
    inline RGBA *getnc_texBufHeadPtr(){
        return &(this->texBuf.at(0));
    }

    inline const std::vector<RGBA> &get_texBuf() const {
        return this->texBuf;
    }

    
    inline const MapAltitude &get_mapEntAlti( size_t idx_ ){
        tprAssert( idx_ < this->mapEntAltis.size() );
        return this->mapEntAltis.at(idx_);
    }

    inline const std::vector<MapAltitude> &get_mapEntAltis() const {
        return this->mapEntAltis;
    }


private:
    std::vector<RGBA>         texBuf {};
    std::vector<MapAltitude>  mapEntAltis {};
};


#endif 

