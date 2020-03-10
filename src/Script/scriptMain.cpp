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
 * 
 *   在目前阶段，script 已经被严重弱化了
 *   现在的游戏，已经越来越像一个 整体性的程序了
 * ----------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "MapTexture.h" 
#include "esrc_behaviour.h" 

//-------------------- Script --------------------//
#include "Script/json/json_all.h"


using namespace std::placeholders;


//----------- 需要注册到 engine 的函数 ---------------//
extern void onGoSpeciesIds_SignUp();
extern void onStart_test();

/* ===========================================================
 *                     scriptMain 
 * -----------------------------------------------------------
 * -- script Enter Point
 * -- 常见功能： 将一些 脚本层函数 注册到 引擎中。
 */
void scriptMain(){

    //------- Awakes -------//

    esrc::get_behaviour().signUp_Awakes( std::bind( &onGoSpeciesIds_SignUp ) );

    //------- Starts -------//
    esrc::get_behaviour().signUp_Starts( std::bind( &onStart_test ) );

    



}




