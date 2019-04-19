/*
 * ========================= scriptMain.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   script／脚本区 入口函数
 * 
 *   - 本文件中，必须存在 void scriptMain(); 函数
 *    （如果这个函数被丢弃，游戏也将无法运行）
 * ----------------------------
 */

//-------------------- CPP --------------------//
#include <functional> 

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "MapTexture.h" 
#include "esrc_behaviour.h" 

//-------------------- Script --------------------//


using namespace std::placeholders;


//----------- 需要注册到 engine 的函数 ---------------//
extern void onGoSpecIds_SignUp();
extern void onStart_test();

/* ===========================================================
 *                     scriptMain 
 * -----------------------------------------------------------
 * -- script Enter Point
 * -- 常见功能： 将一些 脚本层函数 注册到 引擎中。
 */
void scriptMain(){

    //------- Awakes -------//
    esrc::behaviour.signUp_Awakes( std::bind( &onGoSpecIds_SignUp ) ); //-- 这个函数可能被丢弃--

    //------- Starts -------//
    esrc::behaviour.signUp_Starts( std::bind( &onStart_test ) );


}




