/*
 * ===================== MapField.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   4*4mapent 构成一个 field   [第二版]
 * ----------------------------
 */
#ifndef TPR_MAP_FIELD_H
#define TPR_MAP_FIELD_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "EcoSysPlanType.h"
#include "MapCoord.h"
#include "chunkKey.h"
#include "sectionKey.h"
#include "fieldKey.h"
#include "RGBA.h"
#include "MapAltitude.h"
#include "occupyWeight.h"
#include "fieldBorderSetId_t.h"
#include "Density.h"

#include "tprDebug.h"


class MemMapEnt;


//-- 4*4mapent 构成一个 field -- [just mem]
//  另一个身份是 “距离场” 
//  每一个 chunk 都要存储 8*8 个 MapField数据。
//  ------
class MapField{
public:
    MapField() = default;

    void init( const IntVec2 &anyMPos_ );

    inline bool is_land() const {
        //return (this->minMapAlti.is_land() &&
        //        this->nodeMapAlti.is_land() );
                    //- 存在一处诡异的bug：当改成 nodeAlti.is_inland()
                    //  地图上种植的树木个数会大幅度减少
                    //  未修改...
        return ( this->nodeMapAlti.is_inland_2() );
                    //- 尝试解决 水域中央生成树 的bug
    }

    //------- set -------//    
    inline void reflesh_min_and_max_altis( const MapAltitude &mapAlti_ ){
        if( mapAlti_ < this->minMapAlti ){
            this->minMapAlti = mapAlti_;
        }
        if( mapAlti_ > this->maxMapAlti ){
            this->maxMapAlti = mapAlti_;
        }
    }

    void set_nodeAlti_2( const std::vector<std::unique_ptr<MemMapEnt>> &chunkMapEnts_ );

    //------- get -------//
    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }
    inline const fieldKey_t &get_fieldKey() const {
        return this->fieldKey;
    }
    inline const IntVec2& get_ppos() const {
        return this->mcpos.get_ppos();
    }
    inline const Density &get_density() const {
        return this->density;
    }
    inline const sectionKey_t &get_ecoObjKey() const {
        return this->ecoObjKey;
    }
    inline const fieldBorderSetId_t &get_fieldBorderSetId() const {
        return this->fieldBorderSetId;
    }
    inline const MapAltitude &get_minMapAlti() const {
        return this->minMapAlti;
    }
    inline const MapAltitude &get_maxMapAlti() const {
        return this->maxMapAlti;
    }
    inline const MapAltitude &get_nodeMapAlti() const {
        return this->nodeMapAlti;
    }
    inline const IntVec2 &get_nodeMPos() const {
        return this->nodeMPos;
    }
    inline const occupyWeight_t &get_occupyWeight() const {
        return this->occupyWeight;
    }
    inline const double &get_weight() const {
        return this->weight;
    }

private:
    void init_nodeMPos();
    void init_occupyWeight();
    void assign_field_to_4_ecoObjs();

    //====== vals =======//
    //----- 一阶数据 / first order data ------//
    MapCoord    mcpos    {};    //- field左下角mcpos
                                // 这么存储很奢侈，也许会在未来被取消...
                                // anyMPos_2_fieldMPos() 
    fieldKey_t  fieldKey {}; 

    IntVec2     nodeMPos {};    //- 距离场点 mpos (4*4 mapent 中的一个点) （均匀距离场）
                                //- 绝对 mpos 坐标。

    glm::dvec2  FDPos {};    //- field-mpos 除以 ENTS_PER_FIELD 再累加一个 随机seed
                            // 这个值仅用来 配合 simplex-noise 函数使用

    double   originPerlin {}; //- perlin 原始值 [-1.0, 1.0]
    double   weight {};  //- 根据 perlin 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t       occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 

    fieldBorderSetId_t  fieldBorderSetId {}; 

    sectionKey_t        ecoObjKey {};

    Density             density {};
    
    //----- 三阶数据 / third order data ------//
    MapAltitude  minMapAlti { 100.0 };  
    MapAltitude  maxMapAlti { -100.0 }; //- field 拥有的所有 mapent 的 中点pix 的，alti最低值，最大值
                                  //- 默认初始值 需要反向设置
                                  //  通过这组值，来表达 field 的 alti 信息
                                  //  ---------
                                  //  但在实际使用中，这组值并不完善，chunk边缘field 的 这组alti值往往无法被填完
                                  //  就要开始 种go。此时很容易把 go 种到水里。
    
    MapAltitude  nodeMapAlti {}; //- nodeMPos 点的 alti 值

    //===== flags =====//
    bool  isNodeMapAltiSet {false}; // tmp 只能被设置一次

};



#endif 

