/*
 * ========================= Move.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    专门管理 GameObj实例 的 位移运动
 * ----------------------------
 */
#ifndef _TPR_MOVE_H_
#define _TPR_MOVE_H_


//-------------------- Engine --------------------//
#include "CrossState.h" 


//-- 依赖代码 --
class GameObj;


//-- 实际上，Move只计算 go实例 的 currentVelocity --
//-- 并不影响 go实例的 pos -- 
class Move{
public:
    Move() = default;

    inline void init( GameObj *_goPtr ){ //-- MUST --
        goPtr = _goPtr;
    }

    void RenderUpdate( CrossState _newCs ); 

    //-- flags --
    //-- 当前是否 正朝某个方向 移动 --
    bool is_left_moving   {false};
    bool is_right_moving  {false};
    bool is_up_moving     {false};
    bool is_down_moving   {false};

    float      speedStandard {1.5f};       //- 单一方向的 速度上限（速度标准）

    CrossState  currentCrossState {0,0};  //- 当前 十字键状态 

    



private:
    GameObj  *goPtr {nullptr}; //- 每个 move实例 都属于一个 go实例
                                //  两者 强关联，共存亡

    





};

#endif

