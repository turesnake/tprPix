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

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "EcoSysType.h"
#include "MapCoord.h"
#include "chunkKey.h"
#include "sectionKey.h"
#include "fieldKey.h"
#include "RGBA.h"
#include "Altitude.h"
#include "occupyWeight.h"
#include "fieldBorderSetId_t.h"
#include "Density.h"


//-- 4*4mapent 构成一个 field -- [just mem]
//  另一个身份是 “距离场” 
//  每一个 chunk 都要存储 16*16个 MapField数据。
//  ------
//  
class MapField{
public:

    void init( const IntVec2 &_anyMPos );

    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }
    inline const IntVec2& get_ppos() const {
        return this->mcpos.get_ppos();
    }

    //----- 一阶数据 / first order data ------//
    MapCoord    mcpos    {};    //- field左下角mcpos
                                // 这么存储很奢侈，也许会在未来被取消...
                                // anyMPos_2_fieldMPos() 
    fieldKey_t  fieldKey {}; 

    IntVec2     nodeMPos {};    //- 距离场点 mpos (4*4 mapent 中的一个点) （均匀距离场）
                                //- 绝对 mpos 坐标。

    sectionKey_t  ecoSysInMapKey {};

    int          lColorOff_r {}; //- 颜色偏移，[-10,10] 之间
    int          lColorOff_g {}; 
    int          lColorOff_b {};

    float weight {};  //- 根据 perlin 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t  occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 

    fieldBorderSetId_t  fieldBorderSetId {}; 

    Density         density {};


    //----- 三阶数据 / third order data ------//
    
    Altitude  minAlti { 100.0 };  
    Altitude  maxAlti { -100.0 }; //- field 拥有的所有 mapent 的 中点pix 的，alti最低值，最大值
                                  //- 默认初始值 需要反向设置
                                  //  通过这组值，来表达 field 的 alti 信息


    //====== flags =======//


private:

    glm::vec2  FDPos {};    //- field-mpos 除以 ENTS_PER_FIELD 再累加一个 随机seed
                            // 这个值仅用来 配合 simplex-noise 函数使用

    float      originPerlin {}; //- perlin 原始值 [-1.0, 1.0]


    void init_nodeMPos();
    void init_lColorOff();
    void init_occupyWeight();

    void assign_field_to_4_ecoSysInMaps();
    
};



#endif 

