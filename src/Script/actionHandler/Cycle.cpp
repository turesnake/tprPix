/*
 * ======================= Cycle.cpp ========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器
 * ----------------------------
 */
#include "Cycle.h"

//-------------------- C --------------------//
#include <cassert> //- assert

using namespace std::placeholders;


namespace actionHdlr{//------------- namespace ActionHdlr ----------------

namespace{
    int   retIdx {0}; //- method return val store place
}

/* ===========================================================
 *                          bind
 * -----------------------------------------------------------
 */
void Cycle::bind(  ActionHandler *_ahPtr,
                int _frames,
                int _enterIdx,
                int _step ){

    //-- 绑定 基础ah实例 --
    //  在此之前，外部代码需要自行生成 基础ah类实例
    ahPtr = _ahPtr;

    //-- type --
    ahPtr->typeId = Cycle::typeId;

    //-- binary --
    ahPtr->binary.clear();
    ahPtr->binary.resize( sizeof(Cycle_Binary) );
    bp = (Cycle_Binary*)&(ahPtr->binary[0]); 
    //===================================//

    bp->frames = _frames;
    bp->enterIdx = _enterIdx;
    bp->currentIdx = _enterIdx;
    bp->step = _step;

    bp->lastIdx = _enterIdx;
    bp->updates = 0;
    bp->step_new = 0;
    bp->is_step_change = false;


    //----- sign up callback funcs -----
    //-- 故意将 首参数this 绑定到 保留类实例 ah3 身上
    ahPtr->funcs.insert({ "update", 
            std::bind( &Cycle::update, &ah_cycle, _1 )
            });

    ahPtr->funcs.insert({ "get_currentIdx", 
            std::bind( &Cycle::get_currentIdx, &ah_cycle, _1 )
            });

    ahPtr->funcs.insert({ "set_step", 
            std::bind( &Cycle::set_step, &ah_cycle, _1, _2, _3 )
            });
}


/* ===========================================================
 *                        update
 * -----------------------------------------------------------
 * -- int update( ActionHandler *_ahPtr )
 * -- 需要在 每一视觉帧 调用
 * -- 若 某段时间 不主动调用本函数，动画将陷入停滞（并不会严格对应到全局总时间帧）
 * -- 顺带返回  currentIdx 的值
 */
void *Cycle::update( ActionHandler *_ahPtr ){

    //-- check type -----
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary[0]);
    //=====================================//

    bp->updates++;
    //-------
    int steps = bp->updates / bp->step;
    int newIdx = (bp->lastIdx + steps)%bp->frames;
    //- 发生了 画面帧 切换 -
    if( bp->currentIdx != newIdx ){
        bp->currentIdx = newIdx;
        if( bp->is_step_change == true ){
            bp->is_step_change = false;
            bp->step = bp->step_new;
            bp->lastIdx = bp->currentIdx;
            bp->updates = 0;
        }
    }
    //-- ret --
    retIdx = bp->currentIdx;
    return (void*)&retIdx; //- 确保外部按规则解指针
}


/* ===========================================================
 *                   get_currentIdx
 * -----------------------------------------------------------
 * -- int get_currentIdx( ActionHandler *_ahPtr )
 */
void *Cycle::get_currentIdx( ActionHandler *_ahPtr ){

    //-- check type -----
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary[0]);
    //=====================================//

    //-- ret --
    retIdx = bp->currentIdx;
    return (void*)&retIdx; //- 确保外部按规则解指针
}


/* ===========================================================
 *                   set_step
 * -----------------------------------------------------------
 * -- void set_step( ActionHandler *_ahPtr, int _step ){
 * -- 重设 step 将清零 updates，并改写 lastIdx。
 * -- 通过不停地 重设 step，可以实现 动画 带有节奏地播放
 * -- 重设 step 不会立即起效，而是先登记下来，等下一 画面帧切换时，再更新 step
 */
void *Cycle::set_step( ActionHandler *_ahPtr, void *_buf, int *_len ){

    //-- check type -----
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary[0]);
    //=====================================//

    //-- get int _step --
    assert( *_len == sizeof(int) );
    int _step = *((int*)_buf);

    assert( _step > 0 );
    if( _step == bp->step_new ){
        return;
    }
    bp->step_new = _step;

    if( bp->is_step_change == true ){
        return;
    }else{
        bp->is_step_change = true;
    }
    //-- ret --
    return nullptr;
}





}//----------------- namespace ActionHdlr: end -------------------

