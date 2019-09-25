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
#include "tprCast.h"

#include "tprDebug.h"


class MemMapEnt;


//-- 4*4mapent 构成一个 field -- [just mem]
//  另一个身份是 “距离场” 
//  每一个 chunk 都要存储 8*8 个 MapField数据。
//  ------
class MapField{
public:
    MapField() = default;

    void init( IntVec2 anyMPos_ );

    inline bool is_land() const noexcept{
        //return (this->minMapAlti.is_land() &&
        //        this->nodeMapAlti.is_land() );
                    //- 存在一处诡异的bug：当改成 nodeAlti.is_inland()
                    //  地图上种植的树木个数会大幅度减少
                    //  未修改...
        return ( this->nodeMapAlti.is_inland_2() );
                    //- 尝试解决 水域中生成树的 bug
    }

    //------- set -------//    
    inline void reflesh_min_and_max_altis( MapAltitude mapAlti_ )noexcept{
        if( mapAlti_ < this->minMapAlti ){
            this->minMapAlti = mapAlti_;
        }
        if( mapAlti_ > this->maxMapAlti ){
            this->maxMapAlti = mapAlti_;
        }
    }

    void set_nodeAlti_2( const std::vector<std::unique_ptr<MemMapEnt>> &chunkMapEnts_ );

    //------- get -------//
    inline IntVec2      get_mpos() const noexcept{ return this->mcpos.get_mpos(); }
    inline IntVec2      get_ppos() const noexcept{ return this->mcpos.get_ppos(); }
    inline MapAltitude  get_minMapAlti() const noexcept{ return this->minMapAlti; }
    inline MapAltitude  get_maxMapAlti() const noexcept{ return this->maxMapAlti; }
    inline MapAltitude  get_nodeMapAlti() const noexcept{ return this->nodeMapAlti; }
    inline IntVec2      get_nodeMPos() const noexcept{ return this->nodeMPos; }
    inline fieldKey_t   get_fieldKey() const noexcept{ return this->fieldKey; }
    inline Density      get_density() const noexcept{ return this->density; }
    inline sectionKey_t         get_ecoObjKey() const noexcept{ return this->ecoObjKey; }
    inline fieldBorderSetId_t   get_fieldBorderSetId() const noexcept{ return this->fieldBorderSetId; }
    inline occupyWeight_t       get_occupyWeight() const noexcept{ return this->occupyWeight; }
    inline double       get_weight() const noexcept{ return this->weight; }
    inline double       get_uWeight() const noexcept{ return this->uWeight; }


    inline glm::dvec2 get_nodeDPos() const noexcept{
        return (mpos_2_dpos(this->nodeMPos) + this->nodeDPosOff);
    }

    inline size_t calc_fieldIdx_in_chunk() const noexcept{
        IntVec2 off = this->get_mpos() - anyMPos_2_chunkMPos(this->get_mpos());
        off.x /= ENTS_PER_FIELD;
        off.y /= ENTS_PER_FIELD;

        tprAssert( (off.x>=0) && (off.y>=0) );
        return cast_2_size_t( off.y * FIELDS_PER_CHUNK + off.x );
    }

private:
    void init_nodeMPos_and_nodeDPosOff();
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
                                //  实际上，为了避免相邻 field 的 go紧密相连，
                                //  nodeMPos 只在 {0,1,2} 之间分配

    glm::dvec2  nodeDPosOff {}; //- 对 nodeMPos 的一个补充，用于在map 种植 go 时，pos更加随机性
                                //  [ 0.0, 54.0 ]

    glm::dvec2  FDPos {};    //- field-dpos 除以 ENTS_PER_FIELD 再累加一个 随机seed
                            // 这个值仅用来 配合 simplex-noise 函数使用

    double   originPerlin {}; //- perlin 原始值 [-1.0, 1.0]
    double   weight {}; //- 根据 perlin 生成的 权重值。[-100.0, 100.0]
    double   uWeight {}; // [0.0, 100.0]

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

