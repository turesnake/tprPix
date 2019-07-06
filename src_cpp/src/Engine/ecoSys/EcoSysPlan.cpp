/*
 * ========================= EcoSysPlan.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "EcoSysPlan.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "random.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 


namespace ecoSysPlan_inn {//-------- namespace: ecoSysPlan_inn --------------//

    //- 根据 density.lvl, 颜色递进（逐渐加深）
    int off_r  { -11 };
    int off_g  { -8 };
    int off_b  { -7 };

}//------------- namespace: ecoSysPlan_inn end --------------//

/* ===========================================================
 *        init_landColor_onlyHighLand
 * -----------------------------------------------------------
 * -- landColor 上色方案：
 *    让 density.lvl: 1，2，3 颜色逐渐加深。
 *    其余区域 保持原色 
 */
void EcoSysPlan::init_landColor_onlyHighLand( const RGBA &baseColor_ ){

    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minLvl(); i<=Density::get_maxLvl(); i++ ){ //- [-3,3]
        (i <= 0) ?
            this->landColors.at( Density::lvl_2_idx(i) ) = baseColor_ :
            this->landColors.at( Density::lvl_2_idx(i) ) = baseColor_.add(  i * ecoSysPlan_inn::off_r, 
                                                                            i * ecoSysPlan_inn::off_g, 
                                                                            i * ecoSysPlan_inn::off_b, 
                                                                            0 );
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
void EcoSysPlan::init_landColor_doubleDeep( const RGBA &baseColor_ ){

    int absI {}; 
    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minLvl(); i<=Density::get_maxLvl(); i++ ){ //- [-3,3]
        absI = std::abs(i);
        this->landColors.at( Density::lvl_2_idx(i) ) = baseColor_.add(  absI * ecoSysPlan_inn::off_r, 
                                                                        absI * ecoSysPlan_inn::off_g, 
                                                                        absI * ecoSysPlan_inn::off_b, 
                                                                        0 );
    }
}


/* ===========================================================
 *          init_landColor_twoPattern
 * -----------------------------------------------------------
 * -- landColor 上色方案：
 *    所有 密度大于等于 density_.lvl 的走 _color_1。
 *    剩下区域 走 _color_2
 */
void EcoSysPlan::init_landColor_twoPattern( const Density &density_high_,
                                    const RGBA &color_high_,
                                    const RGBA &color_low_,
                                    bool  is_goDeep_high_,
                                    bool  is_goDeep_low_ ){

    int    absI {};
    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minLvl(); i<=Density::get_maxLvl(); i++ ){ //- [-3,3]
        absI = std::abs(i);
        if( i >= density_high_.get_lvl() ){ //- high
            is_goDeep_high_ ?
                this->landColors.at( Density::lvl_2_idx(i) ) = color_high_.add( absI * ecoSysPlan_inn::off_r, 
                                                                                absI * ecoSysPlan_inn::off_g, 
                                                                                absI * ecoSysPlan_inn::off_b, 
                                                                                0 ) :
                this->landColors.at( Density::lvl_2_idx(i) ) = color_high_;
        }else{ //- low
            is_goDeep_low_ ?
                this->landColors.at( Density::lvl_2_idx(i) ) = color_low_.add(  absI * ecoSysPlan_inn::off_r, 
                                                                                absI * ecoSysPlan_inn::off_g, 
                                                                                absI * ecoSysPlan_inn::off_b, 
                                                                                0 ) :
                this->landColors.at( Density::lvl_2_idx(i) ) = color_low_;
        }
    }
}


/* ===========================================================
 *             init_densityDatas
 * -----------------------------------------------------------
 */
void EcoSysPlan::init_densityDatas( double densitySeaLvlOff_, 
                                const std::vector<double> &datas_ ){

    tprAssert( !this->is_densityDivideVals_init );
    this->densitySeaLvlOff = densitySeaLvlOff_;

    //-- 确认个数不出错 --
    tprAssert( datas_.size() == Density::get_idxNum()-1 );
    //-- 确认 每个元素值 不越界 --
    for( const auto &i : datas_ ){
        tprAssert( (i>=-100.0) && (i<=100.0) );
    }
    //-- 确认 每个元素值 递增 --
    double old {0.0};
    double neo {0.0};
    for( auto i=datas_.cbegin(); i!=datas_.cend(); i++ ){
        if( i == datas_.cbegin() ){
            old = *i;
        }else{
            neo = *i;
            tprAssert( old < neo );
            old = neo;
        }
    }
    //-- 正式搬运 --
    this->densityDivideVals.clear();
    this->densityDivideVals.insert( this->densityDivideVals.end(),
                                    datas_.begin(),
                                    datas_.end() );

    this->is_densityDivideVals_init = true;
}


/* ===========================================================
 *        init_goSpecIdPools_and_applyPercents
 * -----------------------------------------------------------
 */
void EcoSysPlan::init_goSpecIdPools_and_applyPercents(){
    tprAssert( (this->is_goSpecIdPools_init==false) && 
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
void EcoSysPlan::insert(const Density &density_, 
                    double applyPercent_,
                    const std::vector<EcoEnt> &ecoEnts_ ){

    tprAssert( this->is_applyPercents_init ); //- MUST
    this->applyPercents.at(density_.get_idx()) = applyPercent_;

    goSpecId_t  id_l {};
    for( const auto &ent : ecoEnts_ ){
        tprAssert( this->is_goSpecIdPools_init ); //- MUST
        auto &poolRef = this->goSpecIdPools.at(density_.get_idx());
        id_l = ssrc::get_goSpecId(ent.specName);
        poolRef.insert( poolRef.begin(), ent.idNum, id_l );
    }
}


/* ===========================================================
 *               shuffle_goSpecIdPools
 * -----------------------------------------------------------
 * -- 需要调用者 提供 seed
 *    通过这种方式，来实现真正的 伪随机
 */
void EcoSysPlan::shuffle_goSpecIdPools( u32_t seed_ ){

    std::default_random_engine  rEngine; 
    rEngine.seed( seed_ );
    for( auto &poolRef : this->goSpecIdPools ){
        std::shuffle( poolRef.begin(), poolRef.end(), rEngine );
    }
}


