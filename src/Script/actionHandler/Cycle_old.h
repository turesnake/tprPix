/*
 * ========================= Cycle_old.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.28
 *                                        修改 -- 2018.12.28
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器
 * ----------------------------
 */
#ifndef _TPR_ACTION_HANDLER_CYCLE_OLD_H_
#define _TPR_ACTION_HANDLER_CYCLE_OLD_H_

//-------------------- C ----------------------//
#include <cassert> //-- assert

//------------------- Libs --------------------//
#include "tprDataType.h" 


namespace actionHandler{//------- namespace actionHandler -------


//- 适用于： 一个无限循环的动画action --
//  从入口帧开始，周而复始地播放。
//  当外部发生事件，需要退出 本action 时。会立即退出（从任何一帧）
class Cycle_old{
public:
    Cycle_old(  int _frames,
            int _enterIdx,
            int _step ): 
        frames(_frames),
        enterIdx(_enterIdx),
        lastIdx(enterIdx),
        currentIdx(enterIdx),
        step(_step)
        {}

    inline int get_currentIdx() const {
        return  currentIdx;
    }

    //- 需要在 每一视觉帧 调用 --
    //  若 某段时间 不主动调用本函数，动画将陷入停滞（并不会严格对应到全局总时间帧）
    //  顺带返回  currentIdx
    inline int update(){
        updates++;
        //--------
        int steps = updates/step;
        int newIdx = (lastIdx + steps)%frames;
        //- 发生了 画面帧 切换 -
        if( currentIdx != newIdx ){
            currentIdx = newIdx;
            if( is_step_change == true ){
                is_step_change = false;
                step = step_new;
                lastIdx = currentIdx;
                updates = 0;
            }
        }
        return currentIdx;
    }

    //- 重设 step 将清零 updates，并改写 lastIdx。
    //  通过不停地 重设 step，可以实现 动画 带有节奏地播放
    //  重设 step 不会立即起效，而是先登记下来，等下一 画面帧切换时，再更新 step
    inline void reset_step( int _step ){
        assert( _step > 0 );
        if( _step == step_new ){
            return;
        }
        step_new = _step;

        if( is_step_change == true ){
            return;
        }else{
            is_step_change = true;
        }
    }

private:
    int  frames;     //- 画面帧总数（单个动画动作）
    int  enterIdx;   //- 入口帧序号. 这个值永不变
    int  lastIdx;    //-当 step值被重设时，这个值会被对齐到 当时的 currentIdx
    int  currentIdx; //- 当前指向的 画面帧
    int  step;       //- 每切换一帧画面，停留多少 时间帧
    //---------------------
    int  updates {0}; //- 本实例自创建以来，调用 update() 的次数

    int  step_new       {0};     //- 临时存储 新的 step值
    bool is_step_change {false}; //- step 是否被要求更换
};


}//------------ namespace actionHandler: end ----------------
#endif

