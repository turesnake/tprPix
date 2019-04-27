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


class ChunkData{
public:
    ChunkData() = default;

    const std::vector<float> &get_pixAltis() const {
        return this->pixAltis;
    }

    void resize_pixAltis(){
        this->pixAltis.resize( PIXES_PER_CHUNK * PIXES_PER_CHUNK, 0.0 );
    }

    void set_ent_in_pixAltis( size_t _idx, float _val ){
        assert( _idx < this->pixAltis.size() ); //- tmp
        this->pixAltis.at(_idx) = _val;
    }


private:
    std::vector<float> pixAltis {};

};


#endif 

