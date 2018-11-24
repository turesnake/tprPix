/*
 * ========================= game_src.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   全局变量， 
 *   全游戏的 资源，都存储在此 头文件中
 * 
 * ----------------------------
 */
#ifndef _TPR_GAME_SRC_H_
#define _TPR_GAME_SRC_H_

//------------------- SELF --------------------//
#include "Action_SRC.h"

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>
#include <unordered_map>




//----- 动画动作 资源 ------//
inline std::unordered_map< std::string, Action_SRC > action_srcs {};



#endif
