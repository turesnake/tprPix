/*
 * ========================= action_srcs.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.23
 * ----------------------------------------------------------
 *  资源管理:  Action_SRC
 * ----------------------------
 */
#include "Action_SRC.h"
#include "srcs_manager.h"



/* ===========================================================
 *                     action_srcs_load
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 Action_SRC 资源
 */
void action_srcs_load(){

    action_srcs.clear();

    action_srcs.insert( {"dog_ack_01",  Action_SRC{ "/human/human-1-web.P.png", PixVec2{11, 19}, PixVec2{1, 1}, PixVec2{0, 4} } } );










}




