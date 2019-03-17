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
#include "LandWaterEnt.h"
#include "LandWaterPrefabEdge.h"
#include "LandWaterPrefabCorner.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//


//--- landWater 数据部分 ---



class CornerData{
public:
    CornerData( int _x, int _y, QuadType _quadType){
        mposOff.set( _x, _y );
        quadType = _quadType;
    }
    IntVec2   mposOff      {};
    QuadType  quadType     {}; //- 预制件在本section中的部分，在预制件体内，属于哪个扇区 
};

class EdgeData{
public:
    EdgeData( int _x, int _y, QuadType _quadType, bool _is_leftRight ){
        mposOff.set( _x, _y );
        quadType = _quadType;
        is_leftRight = _is_leftRight;
    }
        
    IntVec2   mposOff      {};
    QuadType  quadType     {}; //- 预制件在本section中的部分，在预制件体内，属于哪个扇区 
    bool      is_leftRight {true};
};


inline std::vector<CornerData> allCornerDatas_in_section{ //- section 4端点 mpos偏移
    CornerData{ 0, 0, QuadType::Right_Top },
    CornerData{ ENTS_PER_SECTION, 0, QuadType::Left_Top },
    CornerData{ 0, ENTS_PER_SECTION, QuadType::Right_Bottom },
    CornerData{ ENTS_PER_SECTION, ENTS_PER_SECTION, QuadType::Left_Bottom }
};

inline std::vector<EdgeData> allEdgeDatas_in_section{ //- section 侧边4节点 mpos 偏移
    EdgeData{ 2*ENTS_PER_CHUNK, 0 , QuadType::Right_Top, false }, //- bottom
    EdgeData{ 0, 2*ENTS_PER_CHUNK,  QuadType::Right_Top, true },  //- left
    EdgeData{ 2*ENTS_PER_CHUNK, 4*ENTS_PER_CHUNK, QuadType::Right_Bottom, false }, //- top
    EdgeData{ 4*ENTS_PER_CHUNK, 2*ENTS_PER_CHUNK, QuadType::Left_Bottom, true },  //- right
};

inline std::vector<LandWaterEnt> landWaterEnts_in_section {}; //- 一个section 拥有的全部 landwater数据
                                                    // 暂时存储在此，最后以 chunk为单位，存储到 全局容器中



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


inline std::vector<IntVec2> randWH; //- 元素为 ent在 section 中的 坐标序号: (3,6)这种。
                                    //- 乱序存储，用来实现 merge_ent()


//===== funcs =====//
void build_landWaterEnts( sectionKey_t _sectionKey );



}//----------------- namespace: sectionBuild: end -------------------//
#endif 

