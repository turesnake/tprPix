/*
 * ======================== GoMemState.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.17
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_MEM_STATE_H
#define TPR_GO_MEM_STATE_H

// go 在游戏运行期间的 所有状态
// ---
// 在目前版本中，一个go，一旦进入 WaitForRelease，将永不能被取回。 
// 如果发现，想要新建的 go，正在 WaitForRelease 状态，说明参数设置有问题，直接报错。
// 这能简化很多 问题
// ---
// 每一渲染帧，renderPool 会重新加载 active态的 go，并排序
// 当把一个 go，改成 WaitForRelease，它就已经无法被渲染出来了。
//
// ---
// 有必要再添加一个 WaitForDB ...
//
enum class GoMemState : int{
    NotExist,
    OnCreating,
    Active,
    WaitForRelease,
    OnReleasing
};


#endif 

