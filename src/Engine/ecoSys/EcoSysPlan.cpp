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



namespace ecoSysPlan_inn {//-------- namespace: ecoSysPlan_inn --------------//

    //- 根据 density.lvl, 颜色递进（逐渐加深）
    int off_r  { -11 };
    int off_g  { -8 };
    int off_b  { -7 };

}//------------- namespace: ecoSysPlan_inn end --------------//


//============== static ===============//
ID_Manager  EcoSysPlan::id_manager { ID_TYPE::U32, 0};


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
 *               shuffle_goSpecDataPools
 * -----------------------------------------------------------
 * -- 需要调用者 提供 seed
 *    通过这种方式，来实现真正的 伪随机
 */
void EcoSysPlan::shuffle_goSpecDataPools( u32_t seed_ ){

    std::default_random_engine  rEngine; 
    rEngine.seed( seed_ );

    for( auto &iPair : this->densityPools ){
        iPair.second->shuffle( rEngine );
    }
}


