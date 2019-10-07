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
#include "Density.h"
#include "tprCast.h"
#include "colorTableId.h"

#include "tprDebug.h"


class MemMapEnt;


//-- 4*4mapent 构成一个 field -- [just mem]
//  另一个身份是 “距离场” 
//  每一个 chunk 都要存储 8*8 个 MapField数据。
//  ------
class MapField{
public:
    MapField( IntVec2 anyMPos_ ):
        mcpos( anyMPos_2_fieldMPos(anyMPos_) )
        {
            this->init();
        }
        

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
    inline void reflesh_min_max_altis( MapAltitude minAlti_, MapAltitude maxAlti_ )noexcept{
        if( minAlti_ < this->minMapAlti ){
            this->minMapAlti = minAlti_;
        }
        if( maxAlti_ > this->maxMapAlti ){
            this->maxMapAlti = maxAlti_;
        }
    }

    inline void set_ecoObjKey(sectionKey_t key_)noexcept{ this->ecoObjKey = key_; }
    inline void set_colorTableId(colorTableId_t id_)noexcept{ this->colorTableId = id_; }
    inline void set_density(Density d_)noexcept{ this->density = d_; }
    inline void set_nodeMapAlti(MapAltitude alti_)noexcept{ this->nodeMapAlti = alti_; }
    inline void set_minAlti(MapAltitude alti_)noexcept{ this->minMapAlti = alti_; }
    inline void set_maxAlti(MapAltitude alti_)noexcept{ this->maxMapAlti = alti_; }
    inline void set_isCrossEcoObj( bool b_ )noexcept{ this->isCrossEcoObj = b_; }
    inline void set_isCrossColorTable( bool b_ )noexcept{ this->isCrossColorTable = b_; }

    //------- get -------//
    inline IntVec2      get_mpos() const noexcept{ return this->mcpos.get_mpos(); }
    inline MapAltitude  get_minMapAlti() const noexcept{ return this->minMapAlti; }
    inline MapAltitude  get_maxMapAlti() const noexcept{ return this->maxMapAlti; }
    inline MapAltitude  get_nodeMapAlti() const noexcept{ return this->nodeMapAlti; }
    inline fieldKey_t   get_fieldKey() const noexcept{ return this->fieldKey; }
    inline Density      get_density() const noexcept{ return this->density; }
    inline sectionKey_t         get_ecoObjKey() const noexcept{ return this->ecoObjKey; }
    inline colorTableId_t       get_colorTableId()const noexcept{ return this->colorTableId; }
    inline occupyWeight_t       get_occupyWeight() const noexcept{ return this->occupyWeight; }
    inline double       get_weight() const noexcept{ return this->weight; }
    inline double       get_uWeight() const noexcept{ return this->uWeight; }
    inline glm::dvec2   get_dpos() const noexcept{ return this->mcpos.get_dpos(); }

    inline const glm::dvec2 &get_nodeDPos() const noexcept{ return this->nodeDPos; }
    
    inline glm::dvec2 get_midDPos()const noexcept{ 
        return (this->mcpos.get_dpos() + MapField::halfDPosOff); 
    }
    

    inline size_t calc_fieldIdx_in_chunk() const noexcept{
        IntVec2 off = this->get_mpos() - anyMPos_2_chunkMPos(this->get_mpos());
        off.x /= ENTS_PER_FIELD;
        off.y /= ENTS_PER_FIELD;

        tprAssert( (off.x>=0) && (off.y>=0) );
        return cast_2_size_t( off.y * FIELDS_PER_CHUNK + off.x );
    }

    //===== static =====//
    static const glm::dvec2 halfDPosOff; // field 中点 距左下角 offset

private:
    void init();
    void init_nodeDPos();
    void init_occupyWeight();

    //====== vals =======//
    //----- 一阶数据 / first order data ------//
    MapCoord    mcpos    {};    //- field左下角mcpos
                                // 这么存储很奢侈，也许会在未来被取消...
                                // anyMPos_2_fieldMPos() 
    fieldKey_t  fieldKey {}; 

    glm::dvec2  nodeDPos {}; //- field 内的一个随机点 ,绝对距离 Must align to pix

    glm::dvec2  FDPos {};    //- field-dpos 除以 ENTS_PER_FIELD 再累加一个 随机seed
                            // 这个值仅用来 配合 simplex-noise 函数使用

    double   originPerlin {}; //- perlin 原始值，分布集中在接近 0.0 的区域  [-1.0, 1.0]
    double   weight {};       //- 仅仅是对 originPerlin 的放大，未能改善分布问题 [-100.0, 100.0]
    double   uWeight {};      //- 打乱后的随机值，分布更均匀 [0.0, 97.0]

    occupyWeight_t       occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 

    sectionKey_t        ecoObjKey {};
    colorTableId_t      colorTableId {}; // same as ecoObj.colorTableId
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
    bool  isNodeMapAltiSet      {false}; // tmp 只能被设置一次
    bool  isCrossEcoObj         {false}; // 境内是否跨越 数个 ecoobj
    bool  isCrossColorTable     {false}; // 境内是否跨越 数个 colortable
};




#endif 

