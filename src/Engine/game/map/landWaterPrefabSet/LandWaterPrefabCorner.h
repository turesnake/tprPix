/*
 * ==================== LandWaterPrefabCorner.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#ifndef _TPR_LAND_WATER_PREFAB_CORNER_H_
#define _TPR_LAND_WATER_PREFAB_CORNER_H_


//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "Quad.h"
#include "chunkKey.h"
#include "ID_Manager.h" 
#include "landWaterPrefabId_t.h"
#include "LandWaterPrefabEnt.h"



class LandWaterPrefabCorner{
public:
    landWaterPrefabCornerId_t   id {0};  //- 在实际地图中，会存储每个位置的 预知件id。

    //-- 将预制件数据 一分为四 [left-top] --
    std::vector<LandWaterPrefabEnt> leftBottoms {};
    std::vector<LandWaterPrefabEnt> rightBottoms {};
    std::vector<LandWaterPrefabEnt> leftTops {};
    std::vector<LandWaterPrefabEnt> rightTops {};

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id 
};
//======== static ========//
inline ID_Manager  LandWaterPrefabCorner::id_manager { ID_TYPE::U32, 1};


void clear_for_LandWaterPrefabCorner();
void build_all_mutant_datas_for_LandWaterPrefabCorner();
void push_back_originData_for_LandWaterPrefabCorner( const LandWaterPrefabEnt &_ent );

landWaterPrefabCornerId_t apply_a_rand_landWaterPrefabCornerId();
const std::vector<LandWaterPrefabEnt> &get_landWaterPrefabCorner( landWaterPrefabCornerId_t _id, QuadType _quad );


#endif 

