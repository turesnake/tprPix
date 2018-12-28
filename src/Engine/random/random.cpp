/*
 * ========================= random.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.29
 *                                        修改 -- 2018.11.29
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
#include <iostream>
#include <string>

//------------------- SELF --------------------//
#include "Engine/gl/glob_gl.h" 
#include "Engine/game/PixVec.h" 

#include "Engine/resource/srcs_manager.h" //- 所有资源

using std::string;
using std::cout;
using std::endl;


namespace{//------------- namespace ----------------//

    //-- 随机数引擎实例,没传入种子，所以是伪随机，--
    //   使用时需要用种子打乱它,
    std::default_random_engine dRandEng; 

}//------------------ namespace end ----------------//

/* ===========================================================
 *                      get_dRandEng
 * -----------------------------------------------------------
 */
std::default_random_engine &get_dRandEng(){

    //---------------------//
    //      生成 seed 
    //---------------------//
    unsigned int seed = get_new_seed();
    //-----------------------------//
    //   把种子喂给 引擎，打乱它的状态
    //-----------------------------//
    dRandEng.seed(seed);
    return dRandEng;
}

/* ===========================================================
 *                    get_new_seed
 * -----------------------------------------------------------
 * -- 根据 当前鼠标坐标，和 程序时间，生成一个 新种子
 */
unsigned int get_new_seed(){

    unsigned int seed; //- return;

    PixVec2 mousePos = get_mouse_pos();
    seed = mousePos.x + (mousePos.y*3);

    float tm = src::timer.get_currentTime();
    seed += (unsigned int)(tm * 10000000);
                    //-- glfw 时钟的精度就是这么多位

    return seed;
}




