/*
 * ========================= EcoSys.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "EcoSys.h"

//-------------------- Engine --------------------//
#include "srcs_engine.h"

//============== static ===============//
ID_Manager  EcoSys::id_manager { ID_TYPE::U32, 1};


/* ===========================================================
 *              init_landColor
 * -----------------------------------------------------------
 * -- 简易方案，在未来，会设置更加多变的 颜色
 */
void EcoSys::init_landColor( const RGBA &_baseColor ){

    //- density 大于0的 field，颜色会加深
    int off_r = -11;
    int off_g = -8;
    int off_b = -7;

    this->landColors.resize( Density::get_idxNum() );
    //---
    for( int i=Density::get_minVal(); i<=Density::get_maxVal(); i++ ){ //- [-3,3]
        (i <= 0) ?
            this->landColors.at( Density::lvl_2_idx(i) ) = _baseColor :
            this->landColors.at( Density::lvl_2_idx(i) ) = _baseColor.add( i*off_r, i*off_g, i*off_b, 0 );
    }
}



/* ===========================================================
 *               insert_applyPercent
 * -----------------------------------------------------------
 */
void EcoSys::insert_applyPercent( const Density &_density, float _percent ){
    assert( this->is_applyPercents_init ); //- MUST
    this->applyPercents.at(_density.get_idx()) = _percent;
}



/* ===========================================================
 *              insert_goSpecIdPool
 * -----------------------------------------------------------
 */
void EcoSys::insert_goSpecIdPool( const Density &_density, goSpecId_t _id, size_t _num ){
    assert( this->is_goSpecIdPools_init ); //- MUST
    auto &poolRef = this->goSpecIdPools.at(_density.get_idx());
    poolRef.insert( poolRef.begin(), _num, _id );
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



