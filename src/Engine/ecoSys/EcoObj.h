/*
 * ========================== EcoObj.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ECO_OBJ_H_
#define TPR_ECO_OBJ_H_

//-------------------- C --------------------//
//#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "sectionKey.h"
#include "EcoSysPlanType.h"
#include "Quad.h"
#include "occupyWeight.h"
#include "RGBA.h"
#include "IntVec.h"
#include "GameObjType.h"
#include "Density.h"

class EcoSysPlan;

//- 一个在游戏地图上存在的 实实在在的区域。
//- 在游戏世界中，每个 section左下角，都放置一个 EcoSysPlan 数据集 
//  存储了这个 EcoSysPlan 实例 的各种数据 （用来生成地图）
//  ----
//  这组数据会暂时 长期存储在 mem/disk
class EcoObj{
public:
    EcoObj() = default;

    void init_for_node( sectionKey_t _sectionKey );
    void init_for_regular(  sectionKey_t _sectionKey,
                            const std::vector<sectionKey_t> &_nearby_four_ecoSysPlanIds );

    void init_fstOrder( sectionKey_t _sectionKey );

    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }

    //-- 核心函数 --
    // field 会调用此函数
    // 如果自己是 “纯实例“，周边 ecoObj实例，也会调用此函数
    // param: _randV -- [-100.0, 100.0]
    inline goSpecId_t apply_a_rand_goSpecId( size_t _densityIdx, float _randV ) const {
        size_t randV = static_cast<size_t>(floor( _randV * 5.1f + 971.3f ));
        auto &pool = this->goSpecIdPools.at( _densityIdx );
        return pool.at( randV % pool.size() );
    }

    inline const ecoSysPlanId_t &get_ecoSysPlanId() const {
        return this->ecoSysPlanId;
    }
    inline const EcoSysPlanType &get_ecoSysPlanType() const {
        return this->ecoSysPlanType;
    }
    inline const float &get_applyPercent( const Density &_density ) const {
        return this->applyPercentsPtr->at(_density.get_idx());
    }
    inline const float &get_densitySeaLvlOff() const {
        return this->densitySeaLvlOff;
    }
   inline const std::vector<RGBA> *get_landColorsPtr() const {
       return this->landColorsPtr;
   }
    inline const std::vector<float> *get_densityDivideValsPtr() const {
        return this->densityDivideValsPtr;
    }
    inline const sectionKey_t &get_sectionKey() const {
        return this->sectionKey;
    }
    inline const float &get_weight() const {
        return this->weight;
    }
    inline const occupyWeight_t &get_occupyWeight() const {
        return this->occupyWeight;
    }

    //======== static funcs ========// 
    static void calc_nearFour_node_ecoObjKey(  sectionKey_t _targetKey, 
                                                    std::vector<sectionKey_t> &_container );
               
private:
    void copy_datas_from_ecoSysPlan( EcoSysPlan *_targetEcoPtr );

    //--- 只能被 非node 实例调用 --
    void init_for_no_node_ecoObj( const std::vector<sectionKey_t> &_nearby_four_ecoSysPlanIds );
    
    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    float weight {};  //- 根据 simplexNoise 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t  occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 

    ecoSysPlanId_t  ecoSysPlanId {};
    EcoSysPlanType  ecoSysPlanType  {EcoSysPlanType::Forest};

    //-- 本 ecoObj mpos 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2  oddEven {}; 
    
    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    //  实际数据 存储在 ecosysPlan 实例中，此处仅保存 只读指针 --
    const std::vector<RGBA>   *landColorsPtr {};
    const std::vector<float>  *applyPercentsPtr {}; //- each entry: [0.0, 1.0]
    const std::vector<float>  *densityDivideValsPtr {};  //- 6 ents, each_ent: [-100.0, 100.0]
                        
    //-- 独立数据 --
    std::vector<std::vector<goSpecId_t>> goSpecIdPools {};

    float           densitySeaLvlOff  {0.0f}; 

};


#endif 

