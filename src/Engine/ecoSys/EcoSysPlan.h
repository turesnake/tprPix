/*
 * ========================= EcoSysPlan.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ecosystem plan
 * ----------------------------
 */
#ifndef TPR_ECOSYS_PLAN_H
#define TPR_ECOSYS_PLAN_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "RGBA.h" 
#include "GameObjType.h"
#include "ID_Manager.h" 
#include "EcoSysPlanType.h"
#include "GoSpecData.h"
#include "colorTableId.h"
#include "blueprint.h"
#include "Density.h"

#include "GoAssemblePlan.h"

#include "DensityPool.h"




class Density;


//-- 一种 生态群落 --
//  简易版，容纳最基础的数据
//  在未来一点点丰富细节
class EcoSysPlan{
public:
    EcoSysPlan() = default;

    inline void set_id( ecoSysPlanId_t id_ )noexcept{ this->id = id_; }
    inline void set_type( EcoSysPlanType type_ )noexcept{ this->type = type_; }
    inline void set_colorTableId( colorTableId_t id_ )noexcept{ this->colorTableId = id_; }

    inline void insert_2_villageIdRandPool( blueprint::villageBlueprintId_t id_, size_t num_ )noexcept{
        this->villageIdRandPool.insert( this->villageIdRandPool.end(), num_, id_ );
    }

    inline void insert_2_natureFlooryardIdRandPool( blueprint::yardBlueprintId_t id_, size_t num_ )noexcept{
        this->natureFlooryardIdRandPool.insert( this->natureFlooryardIdRandPool.end(), num_, id_ );
    }

    inline void insert_2_natureFloorDensitys( Density density_ )noexcept{
        auto [insertIt, insertBool] = this->natureFloorDensitys.insert( density_ );
        tprAssert( insertBool );
    }
    
    void init_densityDatas( double densitySeaLvlOff_, const std::vector<double> &datas_ );

    inline DensityPool &create_new_densityPool( Density density_ )noexcept{
        auto [insertIt, insertBool] = this->densityPools.emplace( density_, std::make_unique<DensityPool>() );
        tprAssert( insertBool );
        return *(insertIt->second);
    }

    //-- 确保关键数据 都被初始化 --
    void init_check()noexcept;
    
    inline ecoSysPlanId_t       get_id()const noexcept{ return this->id; }
    inline EcoSysPlanType       get_type()const noexcept{ return this->type; }
    inline colorTableId_t       get_colorTableId()const noexcept{ return this->colorTableId; }    
    inline double               get_densitySeaLvlOff() const noexcept{ return this->densitySeaLvlOff; }
    //-- 主要用来 复制给 ecoObj 实例 --
    inline const std::vector<double> *get_densityDivideValsPtr() const noexcept{ return &(this->densityDivideVals); }

    //-- 临时版本 .........
    inline const std::map<Density, std::unique_ptr<DensityPool>> &
    get_densityPools()const noexcept{
        return this->densityPools;
    }

    inline blueprint::villageBlueprintId_t apply_rand_villageBlueprintId( size_t uWeight_ )const noexcept{
        return this->villageIdRandPool.at( (uWeight_ + 7337507) % this->villageIdRandPool.size() );
    }
    
    inline blueprint::yardBlueprintId_t apply_rand_natureFlooryardId( size_t uWeight_ )const noexcept{
        return this->natureFlooryardIdRandPool.at( (uWeight_ + 71010107) % this->natureFlooryardIdRandPool.size() );
    }

    inline const std::set<Density> *get_natureFloorDensitysPtr()const noexcept{
        return &(this->natureFloorDensitys);
    }

    //======== static ========//
    static ID_Manager  id_manager;
    
private:
    //======== vals ========//
    ecoSysPlanId_t      id      {};
    EcoSysPlanType      type    {EcoSysPlanType::Forest};
    colorTableId_t      colorTableId {};
    double              densitySeaLvlOff  {0.0}; 

    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    std::vector<double> densityDivideVals {}; //- 6 ents, each_ent: [-100.0, 100.0]


    std::map<Density, std::unique_ptr<DensityPool>> densityPools {};


    //----- blueprints -----//
    std::vector<blueprint::villageBlueprintId_t> villageIdRandPool {}; // 随机分配池
                        // village 也应该具备 尺寸区别
                        // 并且用不同尺寸的容器来存储
                        // 未实现  ...

    //-- nature_floorYard --//
    std::vector<blueprint::yardBlueprintId_t> natureFlooryardIdRandPool {}; // nature floor yard
    std::set<Density> natureFloorDensitys {}; // 哪些 density 需要分配 natureFloorYard

    //===== flags =====//
    bool   is_densityDivideVals_init {false};

};

#endif 

