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

class Density;


//-- 在 insert() 函数中做参数 --
class EcoEnt{
public:
    EcoEnt( const std::string &specName_, size_t idNum_ ):
        specName(specName_),
        idNum(idNum_)
        {}
    std::string  specName {};
    size_t       idNum    {};
};


//-- 一种 生态群落 --
//  简易版，容纳最基础的数据
//  在未来一点点丰富细节
class EcoSysPlan{
public:
    EcoSysPlan() = default;

    inline void set_id( ecoSysPlanId_t id_ ){
        this->id = id_;
    }
    inline void set_type( EcoSysPlanType type_ ){
        this->type = type_;
    }

    //--- 几种 landColor 上色方案 --
    void init_landColor_onlyHighLand( const RGBA &baseColor_ );
    void init_landColor_doubleDeep( const RGBA   &baseColor_ );
    void init_landColor_twoPattern( int density_high_lvl_,
                                    const RGBA &color_high_,
                                    const RGBA &color_low_,
                                    bool  is_goDeep_high_,
                                    bool  is_goDeep_low_ );

    void init_goSpecIdPools_and_applyPercents();

    void init_densityDatas( double densitySeaLvlOff_, const std::vector<double> &datas_ );

    void insert(int densityLvl_, 
                double applyPercent_,
                const std::vector<std::unique_ptr<EcoEnt>> &ecoEnts_ );
    void shuffle_goSpecIdPools( u32_t seed_ );

    //-- 确保关键数据 都被初始化 --
    inline void chueck_end(){
        tprAssert( (this->is_goSpecIdPools_init) && 
                (this->is_applyPercents_init) &&
                (this->is_densityDivideVals_init) );
    }
    
    inline const ecoSysPlanId_t &get_id() const {
        return this->id;
    }
    inline const EcoSysPlanType &get_type() const {
        return this->type;
    }
    inline const double &get_densitySeaLvlOff() const {
        return this->densitySeaLvlOff;
    }
    //-- 主要用来 复制给 ecoObj 实例 --
    inline const std::vector<RGBA> *get_landColorsPtr() const {
        return &(this->landColors);
    }
    inline const std::vector<double> *get_applyPercentsPtr() const {
        return &(this->applyPercents);
    }
    inline const std::vector<double> *get_densityDivideValsPtr() const {
        return &(this->densityDivideVals);
    }

    //-- 核心函数，ecoObj 通过此函数，分配组成自己的 idPools --
    // param: randV_ -- [-100.0, 100.0]
    inline goSpecId_t apply_a_rand_goSpecId( size_t densityIdx_, double randV_ ){
        size_t randV = cast_2_size_t(floor( randV_ * 1.9 + 701.7 ));
        auto &pool = this->goSpecIdPools.at( densityIdx_ );
        return pool.at( randV % pool.size() );
    }

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 ecoSysPlanId ( 在.cpp文件中初始化 )
    
private:
    //======== vals ========//
    ecoSysPlanId_t      id      {};
    EcoSysPlanType      type    {EcoSysPlanType::Forest};
    double              densitySeaLvlOff  {0.0}; 

    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    std::vector<RGBA>  landColors {};
    std::vector<double> applyPercents {}; //- each entry: [0.0, 1.0]
    std::vector<double> densityDivideVals {}; //- 6 ents, each_ent: [-100.0, 100.0]
    std::vector<std::vector<goSpecId_t>> goSpecIdPools {};
    
    //===== flags =====//
    bool   is_goSpecIdPools_init     {false};
    bool   is_applyPercents_init     {false};
    bool   is_densityDivideVals_init {false};

};
//============== static ===============//
inline ID_Manager  EcoSysPlan::id_manager { ID_TYPE::U32, 1};

#endif 

