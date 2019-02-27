/*
 * =================== MapChunkInBuild.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#ifndef _TPR_MAP_CHUNK_IN_BUILD_H
#define _TPR_MAP_CHUNK_IN_BUILD_H


//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "EcoSysType.h"
#include "MapCoord.h"

namespace sectionBuild { //------- namespace: sectionBuild ----------//

//-- 仅存储 当前正在build 的 section 的 chunk数据 --
//   需要和 全局chunk 数据 配合使用
class MapChunkInBuild{
public:

    //void build_nearby_nodePPoses();

    //----- 第二阶段生成的数据 ------//
    // 在确认正式 生成某个section时，才会填充此数据
    std::vector<IntVec2> nearby_nodePPoses {}; //- 周边9个chunk 的 距离场点。
                                // 每个 pix／mapent 都要频繁访问此数据，
                                // 还是先暂存起来。
                                // 不需要被存储到 disk

};




}//----------------- namespace: sectionBuild: end -------------------//
#endif 

