/*
 * ==================== LandWaterMaskCorner.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#ifndef _TPR_LAND_WATER_MASK_CORNER_H_
#define _TPR_LAND_WATER_MASK_CORNER_H_


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
#include "landWaterMaskId_t.h"
#include "LandWaterMaskEnt.h"



class LandWaterMaskCorner{
public:
    landWaterMaskCornerId_t   id {0};  //- 在实际地图中，会存储每个位置的 预知件id。

    //-- 将预制件数据 一分为四 [left-top] --
    std::vector<LandWaterMaskEnt> leftBottoms {};
    std::vector<LandWaterMaskEnt> rightBottoms {};
    std::vector<LandWaterMaskEnt> leftTops {};
    std::vector<LandWaterMaskEnt> rightTops {};

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id 
};
//======== static ========//
inline ID_Manager  LandWaterMaskCorner::id_manager { ID_TYPE::U32, 1};


void clear_for_LandWaterMaskCorner();
void build_all_mutant_datas_for_LandWaterMaskCorner();
void push_back_originData_for_LandWaterMaskCorner( const LandWaterMaskEnt &_ent );

landWaterMaskCornerId_t apply_a_rand_landWaterMaskCornerId();
const std::vector<LandWaterMaskEnt> &get_landWaterMaskCorner( landWaterMaskCornerId_t _id, QuadType _quad );


#endif 

