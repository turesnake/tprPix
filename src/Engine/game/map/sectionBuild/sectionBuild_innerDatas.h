/*
 * ============ sectionBuild_innerDatas.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   section build 阶段用到的各种 模块内通用数据／工具
 * -----
 *   本文件 只可被 sectionBuild 内部文件包含。
 * ----------------------------
 */
#ifndef _TPR_SECTION_BUILD_INNER_DATAS_H
#define _TPR_SECTION_BUILD_INNER_DATAS_H

//------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "config.h"
#include "Quad.h"
#include "random.h"
#include "IntVec.h"
#include "EcoSysType.h"
#include "NineBox.h"
#include "Chunk.h"
#include "sectionKey.h"
#include "EcoSysInMap.h"
#include "Section.h"
#include "srcs_engine.h"



namespace sectionBuild { //------- namespace: sectionBuild ----------//


//--- landWater 数据部分 ---


inline Section  *currentSectionPtr    {nullptr};
inline Chunk    *currentChunkPtr      {nullptr};



inline std::default_random_engine  randEngine; //-通用 随机数引擎实例
inline std::uniform_int_distribution<int> uDistribution_regular(0,9999); // [0,999] 通用分布器

inline std::uniform_int_distribution<size_t> uDistribution_poissonLand(1500,3000); //- 泊松点个数
inline std::uniform_int_distribution<size_t> uDistribution_poissonWater(300,1500); //- 泊松点个数

inline int landPercent = 5755; //- [1,9999], 陆地占了多大比例
inline int numMergeLand = 6;  //- 合并 land 函数，被反复执行几次 

//-- 通过调整 land-waters区 柏松分部的个数，可以让世界变得更整体，or更分裂。
//-- 随着向地图边缘探索，waters泊松的个数 将在一个 更大的区间内浮动，进而变得更加不规则。
//-- 不同 生态群落 的 land-waters 分布也不相同
//   将被取代...
//inline int landPoissonTimes  = 2 * 8; //- 陆地 泊松分部 个数（随地图增大而变少）
//inline int waterPoissonTimes = 2 * 6; //- 水域 泊松分部 个数（随地图增大而变多）
inline std::uniform_int_distribution<size_t> uDistribution_landPoissonTimes(  4, 2*8 ); //- 陆地 泊松分部 个数（随地图增大而变少）
inline std::uniform_int_distribution<size_t> uDistribution_waterPoissonTimes( 2, 2*6 ); //- 水域 泊松分部 个数（随地图增大而变多）




//===== funcs =====//
void build_landWaterEnts( sectionKey_t _sectionKey );



}//----------------- namespace: sectionBuild: end -------------------//
#endif 

