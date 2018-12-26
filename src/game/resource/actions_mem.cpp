/*
 * ========================= actions_mem.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.23
 * ----------------------------------------------------------
 *  资源管理:  Action
 * ----------------------------
 */
#include "Action.h"
#include "srcs_manager.h"


#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;



/* ===========================================================
 *                     actions_load
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 Actions 资源
 */
void actions_load(){

    actions.clear();

    //actions.insert( {"human-1",  Action{ "/human/human_1.P.png", PixVec2{13, 21}, PixVec2{4, 1} } } );

        Action ac{ "/human/human_1_2.P.png", PixVec2{13, 21}, PixVec2{2, 2} };
        ac.init();


        ac.debug();



}




