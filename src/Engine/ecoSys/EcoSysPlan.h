/*
 * ========================= EcoSysPlan.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ecosystem plan
 * ----------------------------
 */
#ifndef TPR_ECOSYS_PLAN_H_
#define TPR_ECOSYS_PLAN_H_

//-------------------- C --------------------//
//#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "RGBA.h" 
#include "GameObjType.h"
#include "ID_Manager.h" 
#include "EcoSysPlanType.h"

class Density;

//-- 在 insert() 函数中做参数 --
struct EcoEnt{
    std::string  specName;
    size_t       idNum;
};


//-- 一种 生态群落 --
//  简易版，容纳最基础的数据
//  在未来一点点丰富细节
class EcoSysPlan{
public:
    EcoSysPlan() = default;

    inline void set_id( ecoSysPlanId_t _id ){
        this->id = _id;
    }
    inline void set_type( EcoSysPlanType _type ){
        this->type = _type;
    }

    //--- 几种 landColor 上色方案 --
    void init_landColor_onlyHighLand( const RGBA &_baseColor );
    void init_landColor_doubleDeep( const RGBA &_baseColor );
    void init_landColor_twoPattern( const Density &_density_high,
                                    const RGBA &_color_high,
                                    const RGBA &_color_low,
                                    bool  is_goDeep_high,
                                    bool  is_goDeep_low );

    void init_goSpecIdPools_and_applyPercents();

    void init_densityDatas( float _densitySeaLvlOff, const std::vector<float> &_datas );

    void insert(const Density &_density, 
                float _applyPercent,
                const std::vector<EcoEnt> &_ecoEnts );
    void shuffle_goSpecIdPools( u32_t _seed );

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
    inline const float &get_densitySeaLvlOff() const {
        return this->densitySeaLvlOff;
    }
    //-- 主要用来 复制给 ecoObj 实例 --
    inline const std::vector<RGBA> *get_landColorsPtr() const {
        return &(this->landColors);
    }
    inline const std::vector<float> *get_applyPercentsPtr() const {
        return &(this->applyPercents);
    }
    inline const std::vector<float> *get_densityDivideValsPtr() const {
        return &(this->densityDivideVals);
    }

    //-- 核心函数，ecoObj 通过此函数，分配组成自己的 idPools --
    // param: _randV -- [-100.0, 100.0]
    inline goSpecId_t apply_a_rand_goSpecId( size_t _densityIdx, float _randV ){
        size_t randV = static_cast<size_t>(floor( _randV * 1.9f + 701.7f ));
        auto &pool = this->goSpecIdPools.at( _densityIdx );
        return pool.at( randV % pool.size() );
    }

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 ecoSysPlanId ( 在.cpp文件中初始化 )
    
private:
    //======== vals ========//
    ecoSysPlanId_t      id      {};
    EcoSysPlanType      type    {EcoSysPlanType::Forest};
    float               densitySeaLvlOff  {0.0f}; 

    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    std::vector<RGBA>  landColors {};
    std::vector<float> applyPercents {}; //- each entry: [0.0, 1.0]
    std::vector<float> densityDivideVals {}; //- 6 ents, each_ent: [-100.0, 100.0]
    std::vector<std::vector<goSpecId_t>> goSpecIdPools {};
    
    //===== flags =====//
    bool   is_goSpecIdPools_init     {false};
    bool   is_applyPercents_init     {false};
    bool   is_densityDivideVals_init {false};

};
//============== static ===============//
inline ID_Manager  EcoSysPlan::id_manager { ID_TYPE::U32, 1};

#endif 

