/*
 * ========================= IntVec.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.17
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "IntVec.h"

#include <cmath>

//-------------------- Engine --------------------//
#include "mapEntKey.h"
#include "tprCast.h"

#include "simplexNoise.h"
#include "random.h"


// 随机性可能还是不是太好
size_t IntVec2::to_simple_uWeight()const noexcept{

    
    size_t a = cast_2_size_t( mpos_2_key(*this) );
    size_t b = this->x*7 + this->y*17 + 10007; // 素数

    size_t c = this->x*this->x + this->y*this->y;

    c = static_cast<double>( sqrt(static_cast<double>(c)) );

    return (a+c);
    
    

    //--- 更为耗时的 计算方式 ---
    /*
    double freq = 0.17;
    double pnX = simplex_noise2( mpos_2_dpos(*this) * freq ); //- [-1.0, 1.0]
    return blender_the_perlinNoise( pnX, 130617.7, 10000 ); //[0, 9999]
    */
}


