/*
 * ======================= ChunkMemState.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHUNK_MEM_STATE_H
#define TPR_CHUNK_MEM_STATE_H

// chunk 在游戏运行期间的 所有状态
// ---
// OnCreating 和 Active 的临界点，在 esrc::insert_and_init_new_chunk() 中
// 在此之后，chunk部分的数据已经初始化完毕，但是 其上的 go，尚未创建
// ---
// 在目前版本中，一个chunk，一旦进入 WaitForRelease，将永不能被取回。 
// 如果发现，想要新建的 chunk，正在 WaitForRelease 状态，说明参数设置有问题，直接报错。
// 这能简化很多 问题
// ---
// 每一渲染帧，renderPool 会重新加载 active态的 chunk，并排序
// 当把一个chunk，改成 WaitForRelease，它就已经无法被渲染出来了。
//
// ---
// 有必要再添加一个 WaitForDB ...
//
enum class ChunkMemState : int{
    NotExist,
    WaitForCreate,
    OnCreating,
    Active,
    WaitForRelease,
    OnReleasing
};


#endif 

