/*
 * ========================== EcoSysInMap.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#ifndef _TPR_ECOSYS_IN_MAP_H_
#define _TPR_ECOSYS_IN_MAP_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "EcoSysType.h"
#include "Quad.h"
#include "occupyWeight.h"
#include "RGBA.h"
#include "IntVec.h"
#include "GameObjType.h"
#include "Density.h"

class EcoSys;

//- 一个在游戏地图上存在的 实实在在的区域。
//- 在游戏世界中，每个 section左下角，都放置一个 EcoSys数据集 
//  存储了这个 EcoSys实例 的各种数据 （用来生成地图）
//  ----
//  这组数据会暂时 长期存储在 mem/disk
class EcoSysInMap{
public:
    EcoSysInMap() = default;

    void init_for_node( sectionKey_t _sectionKey );
    void init_for_regular(  sectionKey_t _sectionKey,
                            const std::vector<sectionKey_t> &_nearby_four_ecoSysIds );

    void init_fstOrder( sectionKey_t _sectionKey );

    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }

    //-- 核心函数 --
    // field 会调用此函数
    // 如果自己是 “纯实例“，周边 ecosysinmap实例，也会调用此函数
    // param: _randV -- [-100.0, 100.0]
    inline const goSpecId_t apply_a_rand_goSpecId( size_t _densityIdx, float _randV ) const {
        size_t randV = static_cast<size_t>(floor( _randV * 5.1 + 971.3 ));
        auto &pool = this->goSpecIdPools.at( _densityIdx );
        return pool.at( randV % pool.size() );
    }

    inline const ecoSysId_t &get_ecoSysId() const {
        return this->ecoSysId;
    }
    inline const EcoSysType &get_ecoSysType() const {
        return this->ecoSysType;
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
    static void calc_nearFour_node_ecoSysInMapKey(  sectionKey_t _targetKey, 
                                                    std::vector<sectionKey_t> &_container );
               
private:
    void copy_datas_from_ecosys( EcoSys *_targetEcoPtr );

    //--- 只能被 非node 实例调用 --
    void init_for_no_node_ecoSysInMap( const std::vector<sectionKey_t> &_nearby_four_ecoSysIds );
    
    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    float weight {};  //- 根据 simplexNoise 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t  occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 

    ecoSysId_t  ecoSysId {};
    EcoSysType  ecoSysType  {EcoSysType::Forest};

    //-- 本 ecosysInMap mpos 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2  oddEven {}; 
    
    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    //  实际数据 存储在 ecosys 实例中，此处仅保存 只读指针 --
    const std::vector<RGBA>   *landColorsPtr {};
    const std::vector<float>  *applyPercentsPtr {}; //- each entry: [0.0, 1.0]
    const std::vector<float>  *densityDivideValsPtr {};  //- 6 ents, each_ent: [-100.0, 100.0]
                        
    //-- 独立数据 --
    std::vector<std::vector<goSpecId_t>> goSpecIdPools {};

    float           densitySeaLvlOff  {0.0}; 

};


#endif 

