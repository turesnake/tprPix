/*
 * ========================= goSpecIds.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.23
 *                                        修改 -- 2018.12.23
 * ----------------------------------------------------------
 *   将 具象go species 注册到 engine 的 全局变量 goSpecIds 中
 * ----------------------------
 */
//-------------------- Engine --------------------//
#include "Engine/resource/srcs_manager.h" //- 所有资源

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>

using std::cout;
using std::endl;
using std::string;



/* ===========================================================
 *                         
 * -----------------------------------------------------------
 */
void onGoSpecIds_SignUp(){

    //goSpecIds.insert(0); //--- 0号id 保留。
    //==================================//

    

    src::goSpecIds.insert(1);  //- human_1



    src::goSpecIds.insert(1000);  //- Tree_Pine / 松树
    src::goSpecIds.insert(1001);  //- Tree_Oak  / 橡树


    src::goSpecIds.insert(2000);  //- Dig_A / 测试用


    cout << "--- onGoSpecIds_SignUp(). DONE. ---" << endl; 
}










