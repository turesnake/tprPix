/*
 * ==================== LandWaterPrefabEdge.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#ifndef _TPR_LAND_WATER_PREFAB_EDGE_H_
#define _TPR_LAND_WATER_PREFAB_EDGE_H_


//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "Quad.h"
#include "chunkKey.h"
#include "ID_Manager.h" 
#include "landWaterPrefabId_t.h"
#include "LandWaterPrefabEnt.h"


//-- 放在 section 四边 （4*3）个 land-water mapent 侧边预制件 --
// 一个实例，存储一个 预制件
//  这是一些静态数据，每次游戏启动时被加载，之后保持不变
class LandWaterPrefabEdge{
public:
    //======== vals ========//
    landWaterPrefabEdgeId_t    id {0};  //- 在实际地图中，会存储每个位置的 预知件id。

    bool   is_leftRight {true}; //- true  - 左右 
                                //- false - 上下

    //-- 将预制件数据 一分为二：左右／，上／下 --
    std::vector<LandWaterPrefabEnt> left_or_tops {};
    std::vector<LandWaterPrefabEnt> right_or_bottoms {};
    
    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id 
};

//======== static ========//
inline ID_Manager  LandWaterPrefabEdge::id_manager { ID_TYPE::U32, 1};


void clear_for_LandWaterPrefabEdge();
void build_all_mutant_datas_for_LandWaterPrefabEdge();
void push_back_originData_for_LandWaterPrefabEdge( const LandWaterPrefabEnt &_ent );

landWaterPrefabEdgeId_t apply_a_rand_landWaterPrefabEdgeId( bool _is_leftRight );
const std::vector<LandWaterPrefabEnt> &get_landWaterPrefabEdge( landWaterPrefabEdgeId_t _id, QuadType _quad );



#endif 

