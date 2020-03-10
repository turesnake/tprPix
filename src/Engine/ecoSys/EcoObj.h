/*
 * ========================== EcoObj.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ECO_OBJ_H
#define TPR_ECO_OBJ_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "EcoSysPlanType.h"
#include "Quad.h"
#include "occupyWeight.h"
#include "RGBA.h"
#include "GameObjType.h"
#include "Density.h"
#include "GoSpecData.h"
#include "colorTableId.h"
#include "GoDataForCreate.h"
#include "mapEntKey.h"
#include "blueprint.h"
#include "fieldKey.h"
#include "EcoObjBorder.h"

#include "DensityPool.h"


class EcoSysPlan;


//- 一个在游戏地图上存在的 实实在在的区域。
//- 在游戏世界中，每个 section左下角，都放置一个 EcoSysPlan 数据集 
//  存储了这个 EcoSysPlan 实例 的各种数据 （用来生成地图）
//  ----
//  这组数据会暂时 长期存储在 mem/disk
class EcoObj{
public:
    EcoObj() = default;

    void init( sectionKey_t sectionKey_ );

    void init_fstOrder( sectionKey_t sectionKey_ );


    const DensityPool &get_densityPool( Density density_ )const noexcept{
        tprAssert( this->densityPoolsPtr->find(density_) != this->densityPoolsPtr->end() );
        return *(this->densityPoolsPtr->at(density_));
    }


    inline IntVec2              get_mpos() const noexcept{ return this->mpos; }
    inline ecoSysPlanId_t       get_ecoSysPlanId() const noexcept{ return this->ecoSysPlanId; }
    inline EcoSysPlanType       get_ecoSysPlanType() const noexcept{ return this->ecoSysPlanType; }
    inline double               get_densitySeaLvlOff() const noexcept{ return this->densitySeaLvlOff; }
    inline sectionKey_t         get_sectionKey() const noexcept{ return this->sectionKey; }
    inline size_t               get_uWeight() const noexcept{ return this->uWeight; }
    inline occupyWeight_t       get_occupyWeight() const noexcept{ return this->occupyWeight; }
    inline colorTableId_t       get_colorTableId()const noexcept{ return this->colorTableId; }
    inline const std::vector<double> *get_densityDivideValsPtr() const noexcept{ return this->densityDivideValsPtr; }
    inline const EcoObjBorder   *get_ecoObjBorderPtr()const noexcept{ return this->ecoObjBorderPtr; }


    inline blueprint::yardBlueprintId_t get_natureFloorYardId()const noexcept{ return this->natureFloorYardId; }

    inline bool is_find_in_natureFloorDensitys( Density density_ )const noexcept{
        return (this->natureFloorDensitysPtr->find(density_) != this->natureFloorDensitysPtr->end());
    }


    inline std::optional<const GoDataForCreate*> find_majorGoDataForCreatePtr( mapEntKey_t key_ )const noexcept{
        if( this->majorGoDatasForCreate.find(key_) == this->majorGoDatasForCreate.end() ){
            return std::nullopt;
        }else{
            return { this->majorGoDatasForCreate.at(key_).get() };
        }
    }
    inline std::optional<const GoDataForCreate*> find_floorGoDataForCreatePtr( mapEntKey_t key_ )const noexcept{
        if( this->floorGoDatasForCreate.find(key_) == this->floorGoDatasForCreate.end() ){
            return std::nullopt;
        }else{
            return { this->floorGoDatasForCreate.at(key_).get() };
        }
    }

    inline bool is_find_in_artifactFieldKeys( fieldKey_t key_ )const noexcept{
        return (artifactFieldKeys.find(key_) != this->artifactFieldKeys.end() );
    }

    //======== static funcs ========// 
    static void calc_nearFour_node_ecoObjKey(  sectionKey_t targetKey_, 
                                                    std::vector<sectionKey_t> &container_ );
               
private:
    void copy_datas_from_ecoSysPlan( EcoSysPlan *targetEcoPtr_ );
    
    //======== vals ========//
    sectionKey_t  sectionKey {};
    //MapCoord      mcpos  {}; //- [left-bottom]
    IntVec2       mpos {}; // [left-bottom]
                      

    size_t          uWeight {}; // [0, 9999]
    occupyWeight_t  occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 


    ecoSysPlanId_t  ecoSysPlanId {};
    EcoSysPlanType  ecoSysPlanType  {EcoSysPlanType::Forest};

    colorTableId_t  colorTableId {}; // same with ecoPlan.colorTableId

    //-- 本 ecoObj mpos 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2  oddEven {}; 

    const std::vector<double> *densityDivideValsPtr {};  //- 6 ents, each_ent: [-100.0, 100.0]              

    //-- 独立数据 --
    const std::map<Density, std::unique_ptr<DensityPool>> *densityPoolsPtr {nullptr};

                            // 暂时没有确定，是否重分配 densitypools 数据
                            // ...
                            // 目前就是直接借用 ecoplan 里的原版数据

                        
    double           densitySeaLvlOff  {0.0}; 


    //----- blueprint datas -----//
    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> majorGoDatasForCreate {};// key 绝对值
    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> floorGoDatasForCreate {};// key 绝对值
                            
    // 表示 哪些 field 已经存在蓝图数据了（不用生成 nature go 了）
    std::unordered_set<fieldKey_t> artifactFieldKeys {}; // key 绝对值


    blueprint::villageBlueprintId_t villageBlueprintId {};


    //-- nature_floorYard --//
    blueprint::yardBlueprintId_t    natureFloorYardId {};
    const std::set<Density>         *natureFloorDensitysPtr {nullptr};


    const EcoObjBorder *ecoObjBorderPtr {nullptr};


};


#endif 

