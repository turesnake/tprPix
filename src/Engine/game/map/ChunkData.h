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

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Libs --------------------//
#include "tprDataType.h"



class ChunkData{
public:
    ChunkData() = default;


    std::vector<u8_t> pixAltis {};



};





#endif 

