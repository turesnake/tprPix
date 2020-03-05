/*
 * ========================= random.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.29
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    随机数 模块
 * ----------------------------
 */
#ifndef TPR_RANDOM_H
#define TPR_RANDOM_H

//------------------- CPP --------------------//
#include <random>

//------------------- Engine --------------------//
#include "tprCast.h"
#include "tprAssert.h"
#include "IntVec.h"



std::default_random_engine &get_dRandEng();
uint32_t get_new_seed();



//  perlinNoise 往往不够均匀，更容易分布在中线周围
//  执行一次 取模运算，生成一组更均匀的 随机数 
//-- param:
//  noise_:  [-1.0, 1.0]
//  scale_:  [0.0, 100000.0]
//  modBase: some prime number [0, 10000]
inline size_t blender_the_perlinNoise(  double noise_, 
                                        double scale_,
                                        size_t modBase_ ){
    tprAssert(  (noise_>=-1.0) && (noise_<=1.0) &&
                (scale_>0.0) &&
                (scale_ > static_cast<double>(modBase_) * 2.0) );
    double weight = (noise_+1.0) * scale_ + 76123.79; // [0.0, scale_*2.0]
    size_t neo = cast_2_size_t(floor(weight)) % modBase_;
    return neo; // [0, modBase_]
}



size_t calc_simple_mapent_uWeight( IntVec2 mpos_ )noexcept;



#endif
