/*
 * ========================= random.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    随机数 模块
 *  ---------------- 
 *  使用方法：
 *  -    std::default_random_engine dre = get_dRandEng();
 *  -    std::uniform_int_distribution<int> di(0,10);
 *  -    di(dre)
 * ----------------------------
 */
#include "pch.h"
#include "random.h" 


//-------------------- Engine --------------------//
#include "input.h" 
#include "gl_funcs.h" 
#include "mapEntKey.h"

#include "esrc_time.h"


namespace random_inn {//------------- namespace: random_inn ----------------//

    //-- 随机数引擎实例,没传入种子，所以是伪随机，--
    //   使用时需要用种子打乱它,
    std::default_random_engine dRandEng; 

}//------------------ namespace: random_inn end ----------------//

// 当需要生成一次性随机值时，推荐用此方法
std::default_random_engine &get_dRandEng(){

    //---------------------//
    //      生成 seed 
    //---------------------//
    uint32_t seed = get_new_seed();
    //-----------------------------//
    //   把种子喂给 引擎，打乱它的状态
    //-----------------------------//
    random_inn::dRandEng.seed(seed);
    return random_inn::dRandEng;
}

// 根据 当前鼠标坐标，和 程序时间，生成一个 新种子
uint32_t get_new_seed(){

    uint32_t seed {}; //- return;

    IntVec2 mousePos = input::get_mouse_pos();
    seed = static_cast<uint32_t>(mousePos.x + (mousePos.y*3));

    double tm = esrc::get_timer().get_currentTime();
    seed += static_cast<uint32_t>(tm * 10000000);
                    //-- glfw 时钟的精度就是这么多位

    return seed;
}



// simple_uWeight 为每个 mapent生成的，仅用于 蓝图数据分配的 随机数
// 会被各种线程调用，所以务必不要使用 公共数据!!!
size_t calc_simple_mapent_uWeight( IntVec2 mpos_ )noexcept{

    std::default_random_engine dRandEng;
    std::uniform_int_distribution<size_t> uDistribution(1, 10000); // no 0
    //---
    uint32_t ux = static_cast<uint32_t>( mpos_.x );
    uint32_t uy = static_cast<uint32_t>( mpos_.y );
    uint32_t uout = ux ^ (uy << 1); // maybe stackOverFlow, but not care

    dRandEng.seed( static_cast<uint_fast32_t>(uout) );

    //===== 一段丑陋的实现，确保 整合出来的 随机数 randVal，一定不为 0 =====
    size_t v = uDistribution(dRandEng);
    int64_t k = std::abs(static_cast<int64_t>(mpos_2_key(mpos_)));
    size_t randVal = v + static_cast<size_t>(k);

    // 确保一定不为 0
    if( randVal == 0 ){
        randVal = v;
    }

    tprAssert( randVal != 0 );
    return randVal;
}



