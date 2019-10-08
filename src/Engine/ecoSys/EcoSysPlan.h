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

#include "DensityPool.h"




class Density;


//-- 在 insert() 函数中做参数 --
class EcoEnt{
public:
    EcoEnt( const std::string &specName_, 
            std::vector<AnimLabel> &labels_,
            size_t idNum_ ):
        specName(specName_),
        idNum(idNum_)
        {
            this->labels.swap(labels_);
        }
    std::string  specName {};
    size_t       idNum    {};
    std::vector<AnimLabel> labels {};
};




//-- 一种 生态群落 --
//  简易版，容纳最基础的数据
//  在未来一点点丰富细节
class EcoSysPlan{
public:
    EcoSysPlan() = default;

    inline void set_id( ecoSysPlanId_t id_ )noexcept{ this->id = id_; }
    inline void set_type( EcoSysPlanType type_ )noexcept{ this->type = type_; }
    inline void set_colorTableId( colorTableId_t id_ )noexcept{ this->colorTableId = id_; }

    void init_goSpecDataPools_and_applyPercents();

    void init_densityDatas( double densitySeaLvlOff_, const std::vector<double> &datas_ );

    void insert( int densityLvl_, std::unique_ptr<DensityPool> &densityPoolUPtr_ );



    void shuffle_goSpecDataPools( u32_t seed_ );

    //-- 确保关键数据 都被初始化 --
    inline void chueck_end()noexcept{
        tprAssert( (this->is_goSpecDataPools_init) && 
                (this->is_applyPercents_init) &&
                (this->is_densityDivideVals_init) );
    }
    
    inline ecoSysPlanId_t       get_id()const noexcept{ return this->id; }
    inline EcoSysPlanType       get_type()const noexcept{ return this->type; }
    inline colorTableId_t       get_colorTableId()const noexcept{ return this->colorTableId; }
    inline double               get_densitySeaLvlOff() const noexcept{ return this->densitySeaLvlOff; }
    //-- 主要用来 复制给 ecoObj 实例 --
    //inline const std::vector<double> *get_applyPercentsPtr() const noexcept{ return &(this->applyPercents); }
    inline const std::vector<double> *get_densityDivideValsPtr() const noexcept{ return &(this->densityDivideVals); }


    //-- 核心函数，ecoObj 通过此函数，分配组成自己的 idPools --
    // param: randV_ -- [0.0, 100.0]
    /*
    inline const GoSpecData &apply_a_rand_goSpecData( size_t densityIdx_, double randV_ )const noexcept{
            tprAssert( randV_ >= 0.0 );
        size_t randV = cast_2_size_t(floor( randV_ * 1.9 + 701.7 ));
        auto &pool = this->goSpecDataPools.at( densityIdx_ );
        return pool.at( randV % pool.size() );
    }
    */

    
    //-- 临时版本 .........
    inline const std::vector<std::unique_ptr<DensityPool>> &
    get_densityPools()const noexcept{
        return this->densityPools;
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
    //std::vector<double> applyPercents {}; //- each entry: [0.0, 1.0]
    std::vector<double> densityDivideVals {}; //- 6 ents, each_ent: [-100.0, 100.0]

    //std::vector<std::vector<GoSpecData>> goSpecDataPools {};

    std::vector<std::unique_ptr<DensityPool>> densityPools {};

    
    //===== flags =====//
    bool   is_goSpecDataPools_init     {false};
    bool   is_applyPercents_init     {false};
    bool   is_densityDivideVals_init {false};

};

#endif 

