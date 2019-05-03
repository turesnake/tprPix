/*
 * ========================== ChunkData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#ifndef _TPR_CHUNK_DATA_H_
#define _TPR_CHUNK_DATA_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "config.h"
#include "MapTexture.h"
#include "MapAltitude.h"


class ChunkData{
public:
    ChunkData() = default;

    inline void resize_texBuf(){
        this->texBuf.resize(  PIXES_PER_CHUNK_IN_TEXTURE * PIXES_PER_CHUNK_IN_TEXTURE, 
                            RGBA{ 0,0,0,0 } );
    }

    inline void set_mapEntAlti( size_t _idx, const MapAltitude &_alti ){
        assert( _idx < this->mapEntAltis.size() );
        this->mapEntAltis.at(_idx) = _alti;
    }

    inline RGBA *getnc_texBufHeadPtr(){
        return static_cast<RGBA*>( &(this->texBuf.at(0)) );
    }

    inline void init_mapEntAltis(){
        this->mapEntAltis.resize( ENTS_PER_CHUNK * ENTS_PER_CHUNK );
    }

    inline const std::vector<RGBA> &get_texBuf() const {
        return this->texBuf;
    }

    inline MapAltitude &get_mapEntAltiRef( size_t _idx ){
        assert( _idx < this->mapEntAltis.size() );
        return this->mapEntAltis.at(_idx);
    }

    inline const std::vector<MapAltitude> &get_mapEntAltis() const {
        return this->mapEntAltis;
    }

private:
    std::vector<RGBA>      texBuf;
    std::vector<MapAltitude>  mapEntAltis {};
};


#endif 

