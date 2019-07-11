/*
 * ======================= ChunkMemState.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHUNK_STATE_H
#define TPR_CHUNK_STATE_H

// chunk 在游戏运行期间的 所有状态
enum class ChunkMemState : int{
    NotExist,
    OnCreating,
    Active,
    WaitForRelease,
    OnReleasing
};


#endif 

