/*
 * ========================= EcoSys.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "EcoSys.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "Density.h"
#include "esrc_gameSeed.h"

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 



namespace{//-------- namespace: --------------//

    //- 根据 density.lvl, 颜色递进（逐渐加深）
    int off_r = -11;
    int off_g = -8;
    int off_b = -7;


}//------------- namespace: end --------------//

/* ===========================================================
 *        init_landColor_onlyHighLand
 * -----------------------------------------------------------
 * -- landColor 上色方案：
 *    让 density.lvl: 1，2，3 颜色逐渐加深。
 *    其余区域 保持原色 
 */
void EcoSys::init_landColor_onlyHighLand( const RGBA &_baseColor ){

    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minLvl(); i<=Density::get_maxLvl(); i++ ){ //- [-3,3]
        (i <= 0) ?
            this->landColors.at( Density::lvl_2_idx(i) ) = _baseColor :
            this->landColors.at( Density::lvl_2_idx(i) ) = _baseColor.add( i*off_r, i*off_g, i*off_b, 0 );
    }
}


/* ===========================================================
 *        init_landColor_doubleDeep
 * -----------------------------------------------------------
 * -- landColor 上色方案：
 *    让 density.lvl: 1，  2， 3 颜色逐渐加深。
 *    让 density.lvl: -1，-2，-3 颜色逐渐加深。
 * 适合 密林，仅留下小路通行
 */
void EcoSys::init_landColor_doubleDeep( const RGBA &_baseColor ){

    int absI;
    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minLvl(); i<=Density::get_maxLvl(); i++ ){ //- [-3,3]
        absI = abs(i);
        this->landColors.at( Density::lvl_2_idx(i) ) = _baseColor.add( absI*off_r, absI*off_g, absI*off_b, 0 );
    }
}


/* ===========================================================
 *          init_landColor_twoPattern
 * -----------------------------------------------------------
 * -- landColor 上色方案：
 *    所有 密度大于等于 _density.lvl 的走 _color_1。
 *    剩下区域 走 _color_2。
 * 
 */
void EcoSys::init_landColor_twoPattern( const Density &_density_high,
                                    const RGBA &_color_high,
                                    const RGBA &_color_low,
                                    bool  is_goDeep_high,
                                    bool  is_goDeep_low ){

    int    absI;
    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minLvl(); i<=Density::get_maxLvl(); i++ ){ //- [-3,3]
        absI = abs(i);
        if( i >= _density_high.get_lvl() ){ //- high
            is_goDeep_high ?
                this->landColors.at( Density::lvl_2_idx(i) ) = _color_high.add( absI*off_r, absI*off_g, absI*off_b, 0 ) :
                this->landColors.at( Density::lvl_2_idx(i) ) = _color_high;
        }else{ //- low
            is_goDeep_low ?
                this->landColors.at( Density::lvl_2_idx(i) ) = _color_low.add( absI*off_r, absI*off_g, absI*off_b, 0 ) :
                this->landColors.at( Density::lvl_2_idx(i) ) = _color_low;
        }
    }
}


/* ===========================================================
 *             init_densityDatas
 * -----------------------------------------------------------
 */
void EcoSys::init_densityDatas( float _densitySeaLvlOff, 
                                const std::vector<float> &_datas ){

    assert( !this->is_densityDivideVals_init );
    this->densitySeaLvlOff = _densitySeaLvlOff;

    //-- 确认个数不出错 --
    assert( _datas.size() == Density::get_idxNum()-1 );
    //-- 确认 每个元素值 不越界 --
    for( const auto &i : _datas ){
        assert( (i>=-100.0) && (i<=100.0) );
    }
    //-- 确认 每个元素值 递增 --
    float old {0.0};
    float neo {0.0};
    for( auto i=_datas.cbegin(); i!=_datas.cend(); i++ ){
        if( i == _datas.cbegin() ){
            old = *i;
        }else{
            neo = *i;
            assert( old < neo );
            old = neo;
        }
    }
    //-- 正式搬运 --
    this->densityDivideVals.clear();
    this->densityDivideVals.insert( this->densityDivideVals.end(),
                                    _datas.begin(),
                                    _datas.end() );

    this->is_densityDivideVals_init = true;
}


/* ===========================================================
 *        init_goSpecIdPools_and_applyPercents
 * -----------------------------------------------------------
 */
void EcoSys::init_goSpecIdPools_and_applyPercents(){
    assert( (this->is_goSpecIdPools_init==false) && 
            (this->is_applyPercents_init==false) );
    this->goSpecIdPools.resize( Density::get_idxNum(), std::vector<goSpecId_t> {} );
    this->applyPercents.resize( Density::get_idxNum(), 0.0 );
    this->is_goSpecIdPools_init = true;
    this->is_applyPercents_init = true;
}


/* ===========================================================
 *              insert
 * -----------------------------------------------------------
 */
void EcoSys::insert(const Density &_density, 
                    float _applyPercent,
                    const std::vector<EcoEnt> &_ecoEnts ){

    assert( this->is_applyPercents_init ); //- MUST
    this->applyPercents.at(_density.get_idx()) = _applyPercent;

    goSpecId_t  id;
    for( const auto &ent : _ecoEnts ){
        assert( this->is_goSpecIdPools_init ); //- MUST
        auto &poolRef = this->goSpecIdPools.at(_density.get_idx());
        id = ssrc::get_goSpecId(ent.specName);
        poolRef.insert( poolRef.begin(), ent.idNum, id );
    }
}


/* ===========================================================
 *               shuffle_goSpecIdPools
 * -----------------------------------------------------------
 */
void EcoSys::shuffle_goSpecIdPools(){
    for( auto &poolRef : this->goSpecIdPools ){
        std::shuffle( poolRef.begin(), poolRef.end(), esrc::gameSeed.randEngine );
                            //-- 目前，这一步并不符合 “伪随机”...
    }
}



