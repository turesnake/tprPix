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
    MapField() = default;

    void init( const IntVec2 &_anyMPos );

    inline bool is_land() const {
        return (this->minAlti.is_land() &&
                this->nodeAlti.is_land() );
                    //- 存在一处诡异的bug：当改成 nodeAlti.is_inland()
                    //  地图上种植的树木个数会大幅度减少
                    //  未修改...
    }

    //------- set -------//
    inline void set_minAlti( const Altitude &_alti ){
        this->minAlti = _alti;
    }
    inline void set_maxAlti( const Altitude &_alti ){
        this->maxAlti = _alti;
    }
    inline void set_nodeAlti( const Altitude &_alti ){
        this->nodeAlti = _alti;
    }

    //------- set -------//
    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }
    inline const IntVec2& get_ppos() const {
        return this->mcpos.get_ppos();
    }
    inline const Density &get_density() const {
        return this->density;
    }
    inline const sectionKey_t &get_ecoSysInMapKey() const {
        return this->ecoSysInMapKey;
    }
    inline const fieldBorderSetId_t &get_fieldBorderSetId() const {
        return this->fieldBorderSetId;
    }
    inline const Altitude &get_minAlti() const {
        return this->minAlti;
    }
    inline const Altitude &get_maxAlti() const {
        return this->maxAlti;
    }
    inline const Altitude &get_nodeAlti() const {
        return this->nodeAlti;
    }
    inline const IntVec2 &get_nodeMPos() const {
        return this->nodeMPos;
    }
    inline const occupyWeight_t &get_occupyWeight() const {
        return this->occupyWeight;
    }
    inline const float &get_weight() const {
        return this->weight;
    }


private:
    void init_nodeMPos();
    void init_occupyWeight();
    void assign_field_to_4_ecoSysInMaps();

    //====== vals =======//
    //----- 一阶数据 / first order data ------//
    MapCoord    mcpos    {};    //- field左下角mcpos
                                // 这么存储很奢侈，也许会在未来被取消...
                                // anyMPos_2_fieldMPos() 
    fieldKey_t  fieldKey {}; 

    IntVec2     nodeMPos {};    //- 距离场点 mpos (4*4 mapent 中的一个点) （均匀距离场）
                                //- 绝对 mpos 坐标。

    glm::vec2  FDPos {};    //- field-mpos 除以 ENTS_PER_FIELD 再累加一个 随机seed
                            // 这个值仅用来 配合 simplex-noise 函数使用

    float   originPerlin {}; //- perlin 原始值 [-1.0, 1.0]
    float   weight {};  //- 根据 perlin 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t       occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 

    fieldBorderSetId_t  fieldBorderSetId {}; 

    sectionKey_t        ecoSysInMapKey {};

    Density             density {};
    
    //----- 三阶数据 / third order data ------//
    Altitude  minAlti { 100.0 };  
    Altitude  maxAlti { -100.0 }; //- field 拥有的所有 mapent 的 中点pix 的，alti最低值，最大值
                                  //- 默认初始值 需要反向设置
                                  //  通过这组值，来表达 field 的 alti 信息
                                  //  ---------
                                  //  但在实际使用中，这组值并不完善，chunk边缘field 的 这组alti值往往无法被填完
                                  //  就要开始 种go。此时很容易把 go 种到水里。
    
    Altitude  nodeAlti {}; //- nodeMPos 点的 alti 值
};



#endif 

