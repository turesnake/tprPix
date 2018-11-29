/*
 * ========================= random.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.29
 *                                        修改 -- 2018.11.29
 * ----------------------------------------------------------
 *    随机数 模块
 * ---------------- 
 *  使用方法：
 *  -    std::default_random_engine dre = get_dRandEng();
 *  -    std::uniform_int_distribution<int> di(0,10);
 *  -    di(dre)
 * 
 * ----------------------------
 */
#include "random.h" 

#include <iostream>
#include <string>

#include "glob_gl.h" 
#include "PixVec.h" 

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
 * -- 每一次 外部调用 本随机数引擎，都要为其设置一个 新 seed 
 */
std::default_random_engine &get_dRandEng(){

    //---------------------//
    //      生成 seed 
    //---------------------//
    unsigned int seed;

    PixVec2 mousePos = get_mouse_pos();
    seed = mousePos.x + (mousePos.y*3);

    float tm = get_currentTime();
    seed += (unsigned int)(tm * 10000000);
                //-- glfw 时钟的精度就是这么多位

        /*
        cout << "mousePos.x = " << mousePos.x 
            << ", mousePos.y = " << mousePos.y 
            << endl;
        cout << "currentTime = " << tm << endl;
        cout << "seed = " << seed << endl;
        */

    //-----------------------------//
    //   把种子喂给 引擎，打乱它的状态
    //-----------------------------//
    dRandEng.seed(seed);
    return dRandEng;
}







