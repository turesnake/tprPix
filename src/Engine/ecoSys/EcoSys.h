/*
 * ========================= EcoSys.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ecosystem 
 * ----------------------------
 */
#ifndef _TPR_ECOSYS_H_
#define _TPR_ECOSYS_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "RGBA.h" 
#include "Density.h"
#include "GameObjType.h"
#include "ID_Manager.h" 
#include "EcoSysType.h"


//-- 一种 生态群落 --
//  简易版，容纳最基础的数据
//  在未来一点点丰富细节
class EcoSys{
public:
    EcoSys() = default;

    void init_landColor( const RGBA &_baseColor );

    inline void set_id( ecoSysId_t _id ){
        this->id = _id;
    }

    inline void set_type( EcoSysType _type ){
        this->type = _type;
    }

    inline void init_goSpecIdPools(){
        assert( !this->is_goSpecIdPools_init );
        this->goSpecIdPools.resize( Density::get_idxNum(), std::vector<goSpecId_t> {} );
        this->is_goSpecIdPools_init = true;
    }
    inline void init_applyPercents(){
        assert( !this->is_applyPercents_init );
        this->applyPercents.resize( Density::get_idxNum(), 0.0 );
        this->is_applyPercents_init = true;
    }

    void insert_applyPercent( const Density &_density, float _percent );
    void insert_goSpecIdPool( const Density &_density, goSpecId_t _id, size_t _num );
    void shuffle_goSpecIdPools();
    
    inline ecoSysId_t get_id() const {
        return this->id;
    }
    inline const EcoSysType &get_type() const {
        return this->type;
    }

    //-- 主要用来 复制给 ecoSysInMap 实例 --
    inline std::vector<RGBA> &get_landColors(){
        return this->landColors;
    }
    inline std::vector<float> &get_applyPercents(){
        return this->applyPercents;
    }

    //-- 核心函数，ecoSysInMap 通过此函数，分配组成自己的 idPools --
    // param: _randV -- [-100.0, 100.0]
    inline goSpecId_t apply_a_rand_goSpecId( size_t _densityIdx, float _randV ){
        size_t randV = static_cast<size_t>(floor( _randV * 1.9 + 701.7 ));
        auto &pool = this->goSpecIdPools.at( _densityIdx );
        return pool.at( randV % pool.size() );
    }

    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 ecoSysId ( 在.cpp文件中初始化 )
    
private:

    //======== vals ========//
    ecoSysId_t      id      {};
    EcoSysType      type    {EcoSysType::Forest};



    //-- field.nodeAlit.val > 30;
    //-- field.density.lvl [-3, 3] 共 7个池子
    //-- 用 density.get_idx() 来遍历
    std::vector<RGBA>  landColors {};
    std::vector<float> applyPercents {}; //- each entry: [0.0, 1.0]
    std::vector<std::vector<goSpecId_t>> goSpecIdPools {};
    

    //===== flags =====//
    bool   is_goSpecIdPools_init {false};
    bool   is_applyPercents_init {false};

};


#endif 

