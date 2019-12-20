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

//-------------------- CPP --------------------//
#include <vector>
#include <map>
#include <string>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "RGBA.h" 
#include "GameObjType.h"
#include "ID_Manager.h" 
#include "EcoSysPlanType.h"
#include "AnimLabel.h"
#include "GoSpecData.h"
#include "colorTableId.h"
#include "blueprint.h"
#include "Density.h"

#include "MultiGoMesh.h"

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
    inline void pushBack_new_villageBlueprintId( blueprint::villageBlueprintId_t id_ )noexcept{
        this->villageIds.push_back(id_);
    }


    void init_densityDatas( double densitySeaLvlOff_, const std::vector<double> &datas_ );


    inline DensityPool &create_new_densityPool( Density density_ )noexcept{
        auto outPair = this->densityPools.insert({ density_, std::make_unique<DensityPool>() });
        tprAssert( outPair.second );
        return *(outPair.first->second);
    }


    void shuffle_goSpecDataPools( u32_t seed_ );

    //-- 确保关键数据 都被初始化 --
    inline void chueck_end()noexcept{
        tprAssert( this->is_densityDivideVals_init );
    }
    
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

    inline const std::vector<blueprint::villageBlueprintId_t> &
    get_villageBlueprintIds()const noexcept{
        return this->villageIds;
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


    //std::vector<std::unique_ptr<DensityPool>> densityPools {};
    std::map<Density, std::unique_ptr<DensityPool>> densityPools {};


    //----- blueprints -----//
    std::vector<blueprint::villageBlueprintId_t> villageIds {};
                        // village 也应该具备 尺寸区别
                        // 并且用不同尺寸的容器来存储
                        // 未实现  ...
    
    //===== flags =====//


    bool   is_densityDivideVals_init {false};

};

#endif 

