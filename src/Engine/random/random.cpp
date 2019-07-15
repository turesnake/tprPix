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
#include "random.h" 

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "input.h" 
#include "gl_funcs.h" 
#include "IntVec.h" 
#include "esrc_time.h"

using std::string;


//#include "tprDebug.h" //- tmp


namespace random_inn {//------------- namespace: random_inn ----------------//

    //-- 随机数引擎实例,没传入种子，所以是伪随机，--
    //   使用时需要用种子打乱它,
    std::default_random_engine dRandEng; 

}//------------------ namespace: random_inn end ----------------//

/* ===========================================================
 *                      get_dRandEng
 * -----------------------------------------------------------
 * -- 当需要生成一次性随机值时，推荐用此方法
 */
std::default_random_engine &get_dRandEng(){

    //---------------------//
    //      生成 seed 
    //---------------------//
    u32_t seed = get_new_seed();
    //-----------------------------//
    //   把种子喂给 引擎，打乱它的状态
    //-----------------------------//
    random_inn::dRandEng.seed(seed);
    return random_inn::dRandEng;
}

/* ===========================================================
 *                    get_new_seed
 * -----------------------------------------------------------
 * -- 根据 当前鼠标坐标，和 程序时间，生成一个 新种子
 */
u32_t get_new_seed(){

    u32_t seed {}; //- return;

    IntVec2 mousePos = input::get_mouse_pos();
    seed = static_cast<u32_t>(mousePos.x + (mousePos.y*3));

    double tm = esrc::get_timer().get_currentTime();
    seed += static_cast<u32_t>(tm * 10000000);
                    //-- glfw 时钟的精度就是这么多位

    return seed;
}

