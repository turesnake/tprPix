/*
 * ========================= ActionSwitchType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    这个文件 应该放到 script 层
 * ----------------------------
 */
#ifndef _TPR_ACTION_SWITCH_TYPE_H_
#define _TPR_ACTION_SWITCH_TYPE_H_


//-- 如果 元素数量超过 64个，就需要修改 相关内容
enum class ActionSwitchType : int{

    //--- move ----//
    Move_Idle,
    Move_Move


};




#endif 

