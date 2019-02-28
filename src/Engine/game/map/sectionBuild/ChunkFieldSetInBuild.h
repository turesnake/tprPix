/*
 * ============== ChunkFieldSetInBuild.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk Field集合 
 * ----------------------------
 */
#ifndef _TPR_CHUNK_FIELD_SET_IN_BUILD_H
#define _TPR_CHUNK_FIELD_SET_IN_BUILD_H


//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "MapFieldInBuild.h"
#include "ChunkFieldSet.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//


//-- 一个 chunk 拥有的 16*16 个 Field 的集合 --
//  本 class 仅有一个实例，存储在 sectionBuild.h 中
//  每一个正在被创建的 chunk，都临时使用这个 实例
class ChunkFieldSetInBuild{
public:

    void init( ChunkFieldSet *_chunkFieldSetPtr );

    //====== vals =======//
    std::vector<MapFieldInBuild>  fieldsInBuild {}; //- 16*16 个
                //- 建造 chunk 需要的 临时性 Field数据 
                //  本clss 存在的根本意义。

    ChunkFieldSet  *chunkFieldSetPtr {nullptr};
                //- 指向 当前正在build 的 chunk 的 数据
    
};


}//----------------- namespace: sectionBuild: end -------------------//
#endif 

