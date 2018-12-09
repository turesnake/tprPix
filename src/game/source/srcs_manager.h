/*
 * ========================= srcs_manager.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   全游戏的 资源，都存储在此 头文件中
 * 
 * ----------------------------
 */
#ifndef _TPR_GAME_SRC_H_
#define _TPR_GAME_SRC_H_


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>

//------------------- SELF --------------------//
#include "Action_SRC.h"
#include "Player.h" 





//----- Action_SRC 资源 ------//

inline std::unordered_map< std::string, Action_SRC > action_srcs {};
void action_srcs_add();



//----- Player 资源 ------//

inline Player  player; //- 全游戏唯一 Player 实例  






#endif
