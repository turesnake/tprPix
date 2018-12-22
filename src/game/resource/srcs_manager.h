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
#ifndef _TPR_SRCS_MANAGER_H_
#define _TPR_SRCS_MANAGER_H_


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set> 

//------------------- SELF --------------------//
#include "Action_SRC.h"
#include "Player.h" 
#include "TimeBase.h" 
#include "TimeCircle.h" 


//----- time 资源 ------//
inline TimeBase timer {}; //-- 全局时间
inline TimeCircle logicTimeCircle { &timer, 5 }; //- 逻辑时间循环 实例（5帧1周期）



//----- Action_SRC 资源 ------//
inline std::unordered_map< std::string, Action_SRC > action_srcs {};
void action_srcs_load();



//----- GameObj 资源 ------//
//--- mem ---//
inline std::unordered_map<goid_t, GameObj> memGameObjs {}; //- 所有载入内存的 go实例 实际存储区。
 
inline std::unordered_set<goid_t> goids_active   {}; //- 激活组 (身处 激活圈 之内)
inline std::unordered_set<goid_t> goids_inactive {}; //- 未激活组 (身处 激活圈 之外)




//----- globState 资源 ------//
//--- db ---//
void globState_srcs_init();
void globState_srcs_save();


//----- Player 资源 ------//
//--- mem ---//
inline Player  player; //- 全游戏唯一 Player 实例  

//--- db ---//
void player_srcs_init();
void player_srcs_save();








#endif
