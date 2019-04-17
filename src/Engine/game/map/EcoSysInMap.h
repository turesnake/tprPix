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


//- 一个在游戏地图上存在的 实实在在的区域。
//- 在游戏世界中，每个 section左下角，都放置一个 EcoSys数据集 
//  存储了这个 EcoSys实例 的各种数据 （用来生成地图）
//  ----
//  这组数据会暂时 长期存储在 mem/disk
class EcoSysInMap{
public:
    EcoSysInMap() = default;


    void init_fstOrder();

    //-- 只能被 node 实例调用 --
    void init_for_node_ecoSysInMap();

    //--- 只能被 非node 实例调用 --
    void init_for_no_node_ecoSysInMap();
    
    //-- param: _sectionMPos - 本ecoSysInMap.key 所在 section 的 左下角坐标。
    //  仅被 esrc::insert_new_ecoSysInMap() 两函数使用
    inline void set_by_sectionMPos( const IntVec2 &_sectionMPos ){
        this->sectionKey = sectionMPos_2_sectionKey( _sectionMPos );
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }
    inline void set_by_sectionKey( sectionKey_t _sectionKey ){
        this->sectionKey = _sectionKey;
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }

    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }

    //-- 核心函数 --
    // field 会调用此函数
    // 如果自己是 “纯实例“，周边 ecosysinmap实例，也会调用此函数
    // param: _randV -- [-100.0, 100.0]
    inline goSpecId_t apply_a_rand_goSpecId( size_t _densityIdx, float _randV ){
        size_t randV = static_cast<size_t>(floor( _randV * 5.1 + 971.3 ));
        auto &pool = this->goSpecIdPools.at( _densityIdx );
        return pool.at( randV % pool.size() );
    }

    inline ecoSysId_t get_ecoSysId() const {
        return this->ecoSysId;
    }
    inline const EcoSysType &get_ecoSysType() const {
        return this->ecoSysType;
    }

    inline const RGBA &get_landColor( const Density &_density ) const {
        return this->landColors.at(_density.get_idx());
    }

    inline float get_applyPercent( const Density &_density ) const {
        return this->applyPercents.at(_density.get_idx());
    }



    //======== static funcs ========// 
    
    static IntVec2 calc_oddEven( const IntVec2 &_anyMPos ){
        //- 以 section 为单位的 坐标 --
        IntVec2 SPos = floorDiv( _anyMPos, ENTS_PER_SECTION );
        return floorMod( SPos, 2 );
    }

    //-- 核心函数
    static EcoSysInMap *find_or_create_the_ecoSysInMap( sectionKey_t _sectionKey );

    //-- 不该被外部调用
    static EcoSysInMap *find_or_create_target_node_ecoSysInMap( const IntVec2 &_ecosysInMapMPos );


    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    float weight {};  //- 根据 simplexNoise 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t  occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 
    
    //======== flags ========//  
                 
private:
    

    //======== static funcs ========//  

    //======== vals ========//
    ecoSysId_t  ecoSysId {};
    EcoSysType  ecoSysType  {EcoSysType::Forest};


    //-- 本 ecosysInMap mpos 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2  oddEven {}; 


    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    std::vector<RGBA>  landColors {};
    std::vector<float> applyPercents {}; //- each entry: [0.0, 1.0]
    std::vector<std::vector<goSpecId_t>> goSpecIdPools {};


};


#endif 

