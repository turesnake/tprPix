/*
 * ==================== LandWaterMaskEdge.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#ifndef _TPR_LAND_WATER_MASK_EDGE_H_
#define _TPR_LAND_WATER_MASK_EDGE_H_


//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "Quad.h"
#include "chunkKey.h"
#include "ID_Manager.h" 
#include "landWaterMaskId_t.h"
#include "LandWaterMaskEnt.h"


//-- 放在 section 四边 （4*3）个 land-water mapent 侧边预制件 --
// 一个实例，存储一个 预制件
//  这是一些静态数据，每次游戏启动时被加载，之后保持不变
class LandWaterMaskEdge{
public:
    //======== vals ========//
    landWaterMaskEdgeId_t    id {0};  //- 在实际地图中，会存储每个位置的 预知件id。

    bool   is_leftRight {true}; //- true  - 左右 
                                //- false - 上下

    //-- 将预制件数据 一分为二：左右／，上／下 --
    std::vector<LandWaterMaskEnt> left_or_tops {};
    std::vector<LandWaterMaskEnt> right_or_bottoms {};

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id 
};

//======== static ========//
inline ID_Manager  LandWaterMaskEdge::id_manager { ID_TYPE::U32, 1};


void clear_for_LandWaterMaskEdge();
void build_all_mutant_datas_for_LandWaterMaskEdge();
void push_back_originData_for_LandWaterMaskEdge( const LandWaterMaskEnt &_ent );

landWaterMaskEdgeId_t apply_a_rand_landWaterMaskEdgeId( bool _is_leftRight );
const std::vector<LandWaterMaskEnt> &get_landWaterMaskEdge( landWaterMaskEdgeId_t _id, QuadType _quad );



#endif 

