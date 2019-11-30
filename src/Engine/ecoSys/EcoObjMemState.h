/*
 * ======================= EcoObjMemState.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ECO_OBJ_MEM_STATE_H
#define TPR_ECO_OBJ_MEM_STATE_H

// ecoobj 在游戏运行期间的 所有状态
//
enum class EcoObjMemState : int{
    NotExist,
    OnCreating,
    Active,
    OnReleasing // ecoobj 可以被当场删除，不需要在队列中等待
};


#endif 

