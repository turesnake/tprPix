/*
 * ===================== MapField.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   4*4mapent 构成一个 field   [第二版]
 * ----------------------------
 */
#ifndef _TPR_MAP_FIELD_H
#define _TPR_MAP_FIELD_H

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "EcoSysType.h"
#include "MapCoord.h"
#include "chunkKey.h"
//#include "EcoSysInMap.h"
#include "sectionKey.h"
#include "fieldKey.h"
#include "RGBA.h"


//-- 4*4mapent 构成一个 field -- [mem]
//  另一个身份是 “距离场” 
//  每一个 chunk 都要存储 16*16个 MapField数据。
//  而且每个 chunk的这组数据，都是在 chunk被正式生成前 就已经创建了
// （由相邻的chunk）。这组数据会 始终存储在 mem/disk 。需要考虑其 空间开销
//  ------
//  整合版的 field，所有相关数据都在此，非常占容量。
//  但是，最终存储到 disk 中的数据 很有限。 再次加载到mem时，都 "重新生成"
class MapField{
public:

    void init_firstOrderData( const IntVec2 &_anyMPos, const IntVec2 &_chunkMPos );
    void init_secondOrderData();

    //----- 一阶数据 / first order data ------//
    IntVec2     nodePPos {};    //- 距离场点 ppos (20*20pix 中的一个点) （均匀距离场）
                                //- 绝对 ppos 坐标。
    IntVec2     lNodePPosOff {};//- 基于 本chunk 左下角 的 相对 ppos偏移
    MapCoord    mcpos    {};    //- field左下角mcpos
                                 // 这么存储很奢侈，也许会在未来被取消...
                                 // anyMPos_2_fieldMPos() 
    fieldKey_t  fieldKey {}; 

    //----- 二阶数据 / second order data ------//
    // 周边 9个field 一阶数据都准备完毕时，才能开始生成 此数据
    std::unordered_map<fieldKey_t,IntVec2> nearby_field_nodePPoses {}; //- 周边9个field 的 距离场点。
                                // 仅用于 地图生成阶段，不可被存储到 disk
                                

    //----- 三阶数据 / third order data ------//
    u8_t          isLand      {1}; //- 1:land, 0:waters

    RGBA          color {}; //- 临时颜色，tmp...
     
    chunkKey_t    chunkKey {}; 
    sectionKey_t  ecoSysInMapKey {};

    //EcoSysType  ecoSysType  { EcoSysType::Forst }; 

    //====== flags =======//
    bool  is_firstOrderData_set {false};
    bool  is_secondOrderData_set {false};

    bool  is_fieldKey_set       {false};
    bool  is_chunkKey_set       {false};
    bool  is_ecoSysInMapKey_set {false};

    bool  is_color_set  {false}; //- tmp
    
};



#endif 

