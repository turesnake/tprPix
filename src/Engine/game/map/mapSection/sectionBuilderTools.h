/*
 * =================== sectionBuilderTools.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   存放 section生成器的 通用工具
 * -----
 *   本文件 只可被内部文件包含。
 * ----------------------------
 */
#ifndef _TPR_SECTION_BUILDER_H
#define _TPR_SECTION_BUILDER_H


//------------------- Libs --------------------//
#include "tprDataType.h" 


//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"

//----- 存储在 pointMap 中的数据 -----//
inline u8_t LAND   = 1;
inline u8_t WATERS = 0;


//--  等于 MapSection 中的几个值，放这里便于访问 --
inline IntVec2 entWH; //- how mush mapEnts
inline IntVec2 pixWH; //- how mush pixels


//----- 地图生成器参数 ----
inline int landPercent = 5755; //- [1,1000], 陆地占了多大比例
inline int numMergeLand = 10;  //- 合并 land 函数，被反复执行几次 


inline std::vector<u8_t> pointMap; //- each u8 as a mapEnt 
                                   //  记录每个 mapent 的信息，

//-- 通过调整 land-waters区 柏松分部的个数，可以让世界变得更整体，or更分裂。
//-- 随着向地图边缘探索，waters泊松的个数 将在一个 更大的区间内浮动，进而变得更加不规则。
//-- 不同 生态群落 的 land-waters 分布也不相同
//   将被取代...
inline int landPoissonTimes   = 2 * 10; //- 陆地 泊松分部 个数（随地图增大而变多）
inline int watersPoissonTimes = 2 * 8; //- 水域 泊松分部 个数（随地图增大而变多）


inline std::vector<IntVec2> land_poisson_pts; //- 陆地 泊松点 
inline std::vector<IntVec2> waters_poisson_pts; //- 水域 泊松点


inline std::vector<IntVec2> randWH; //- 每个元素 是 ent在 section 中的 坐标序号: (3,6)这种。
                                    //- 乱序存储


inline std::default_random_engine  randEngine; //-随机数引擎实例
inline std::uniform_int_distribution<int> uDistribution(1,10000); 
                                //- 均匀分部器 [1,10000]
inline std::uniform_int_distribution<int> poisDistribution(1000,3000); 
                                //- 均匀分部器 [1000,3000]


void init_sectionBuilderTools(  const IntVec2 &_entWH,
                                const IntVec2 &_pixWH); 


#endif

