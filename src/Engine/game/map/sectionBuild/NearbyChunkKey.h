/*
 * =================== NearbyChunkKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section build态 才需要的 mapent数据
 * ----------------------------
 */
#ifndef _TPR_NEARBY_CHUNK_KEY_H
#define _TPR_NEARBY_CHUNK_KEY_H


//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
//#include "IntVec.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


//- 目标 chunk 周边9个chunk 的基础信息
//- 暂存，用来做快速访问
class NearbyChunkKey{
public:
    u64_t  key       {0};
    bool   isExisted {true}; //- 是否已经生成
};

}//----------------- namespace: sectionBuild: end -------------------//
#endif 

