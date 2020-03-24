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

#include "pch.h"

//-------------------- Engine --------------------//
#include "EcoSysPlanType.h"
#include "MapCoord.h"
#include "chunkKey.h"
#include "sectionKey.h"
#include "fieldKey.h"
#include "RGBA.h"
#include "MapAltitude.h"
#include "occupyWeight.h"
#include "Density.h"
#include "colorTableId.h"


class MemMapEnt;


//-- 4*4mapent 构成一个 field -- [just mem]
class MapField{
public:
    explicit MapField( IntVec2 anyMPos_ ):
        mpos( anyMPos_2_fieldMPos(anyMPos_) )
        {
            this->init();
        }
        
    inline bool is_land() const noexcept{
        return (this->get_maxMapAlti().is_land());
    }
    
    //---------- init ----------//
    inline void init_ecoObjKey(sectionKey_t key_)noexcept{ tprAssert(!this->ecoObjKey.has_value()); this->ecoObjKey = {key_}; }
    inline void init_colorTableId(colorTableId_t id_)noexcept{ tprAssert(!this->colorTableId.has_value()); this->colorTableId = {id_}; }
    inline void init_density(Density d_)noexcept{ tprAssert(!this->density.has_value()); this->density = {d_}; }
    inline void init_nodeMapAlti(MapAltitude alti_)noexcept{ tprAssert(!this->nodeMapAlti.has_value()); this->nodeMapAlti = {alti_}; }
    inline void init_minAlti(MapAltitude alti_)noexcept{ tprAssert(!this->minMapAlti.has_value()); this->minMapAlti = {alti_}; }
    inline void init_maxAlti(MapAltitude alti_)noexcept{ tprAssert(!this->maxMapAlti.has_value()); this->maxMapAlti = {alti_}; }
    inline void init_isCrossEcoObj( bool b_ )noexcept{ tprAssert(!this->isCrossEcoObj.has_value()); this->isCrossEcoObj = {b_}; }
    inline void init_isCrossColorTable( bool b_ )noexcept{ tprAssert(!this->isCrossColorTable.has_value()); this->isCrossColorTable = {b_}; }
    inline void init_uWeight( double v_ )noexcept{ tprAssert(!this->uWeight.has_value()); this->uWeight = {v_}; }

    //---------- get ----------//
    inline IntVec2          get_mpos()const noexcept{ tprAssert(this->mpos.has_value()); return this->mpos.value(); }
    inline glm::dvec2       get_dpos()const noexcept{  return mpos_2_dpos(this->get_mpos()); }
    inline glm::dvec2       get_midDPos()const noexcept{ return (this->get_dpos() + MapField::halfFieldVec2); }
    inline IntVec2          get_midMPos()const noexcept{ return (this->get_mpos() + IntVec2{ HALF_ENTS_PER_FIELD<>, HALF_ENTS_PER_FIELD<> }); }
    inline const glm::dvec2 &get_nodeDPos()const noexcept{ tprAssert(this->nodeDPos.has_value()); return this->nodeDPos.value(); }
    inline MapAltitude      get_nodeMapAlti()const noexcept{ tprAssert(this->nodeMapAlti.has_value()); return this->nodeMapAlti.value(); }
    inline MapAltitude      get_minMapAlti()const noexcept{ tprAssert(this->minMapAlti.has_value()); return this->minMapAlti.value(); }
    inline MapAltitude      get_maxMapAlti()const noexcept{ tprAssert(this->maxMapAlti.has_value()); return this->maxMapAlti.value(); }
    inline fieldKey_t       get_fieldKey()const noexcept{ tprAssert(this->fieldKey.has_value()); return this->fieldKey.value(); }
    inline Density          get_density()const noexcept{ tprAssert(this->density.has_value()); return this->density.value(); }
    inline sectionKey_t     get_ecoObjKey()const noexcept{ tprAssert(this->ecoObjKey.has_value()); return this->ecoObjKey.value(); }
    inline colorTableId_t   get_colorTableId()const noexcept{ tprAssert(this->colorTableId.has_value()); return this->colorTableId.value(); }
    inline occupyWeight_t   get_occupyWeight()const noexcept{ tprAssert(this->occupyWeight.has_value()); return this->occupyWeight.value(); }
    inline size_t           get_uWeight()const noexcept{ tprAssert(this->uWeight.has_value()); return this->uWeight.value(); }
    
private:
    void init();
    void init_nodeDPos( const glm::dvec2 &FDPos_ );
    void init_occupyWeight( const glm::dvec2 &FDPos_ );

    //====== vals =======//
    std::optional<IntVec2>          mpos            {std::nullopt}; // [left-bottom]
    std::optional<fieldKey_t>       fieldKey        {std::nullopt}; 
    std::optional<glm::dvec2>       nodeDPos        {std::nullopt}; //- field 内的一个随机点 ,绝对距离 Must align to pix
    std::optional<size_t>           uWeight         {std::nullopt};      //- 打乱后的随机值，分布更均匀 [0, 9999]
    std::optional<occupyWeight_t>   occupyWeight    {std::nullopt}; //- 抢占权重。 [0,15]
                                                                    //- 数值越高，此 ecosys 越强势，能占据更多fields
                                                                    //- [just mem] 
    std::optional<sectionKey_t>     ecoObjKey       {std::nullopt};
    std::optional<colorTableId_t>   colorTableId    {std::nullopt}; // same as ecoObj.colorTableId
    std::optional<Density>          density         {std::nullopt};
    std::optional<MapAltitude>      nodeMapAlti     {std::nullopt}; //- nodeMPos 点的 alti 值
    std::optional<MapAltitude>      minMapAlti      {std::nullopt};  
    std::optional<MapAltitude>      maxMapAlti      {std::nullopt}; 
                                    //- field 拥有的所有 mapent 的 中点pix 的，alti最低值，最大值
                                    //- 默认初始值 需要反向设置
                                    //  通过这组值，来表达 field 的 alti 信息
                                    //  ---------
                                    //  但在实际使用中，这组值并不完善，chunk边缘field 的 这组alti值往往无法被填完
                                    //  就要开始 种go。此时很容易把 go 种到水里。
    //===== flags =====//
    std::optional<bool>             isCrossEcoObj       {std::nullopt}; // 境内是否跨越 数个 ecoobj
    std::optional<bool>             isCrossColorTable   {std::nullopt}; // 境内是否跨越 数个 colortable

    //===== static =====//
    static constexpr glm::dvec2     halfFieldVec2 { HALF_PIXES_PER_FIELD_D, HALF_PIXES_PER_FIELD_D };
};


#endif 

