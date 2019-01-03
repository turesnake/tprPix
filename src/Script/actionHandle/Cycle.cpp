/*
 * ======================= Cycle.cpp ========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器
 * ----------------------------
 */
#include "Script/actionHandle/Cycle.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <iostream>
#include <string>

using namespace std::placeholders;


using std::string;
using std::cout;
using std::endl;


namespace actionHdle{//------------- namespace ActionHdle ----------------


//--- static member ----
u32  Cycle::typeId {0};


/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 并不是单纯的 bind，还附带了 目标ah实例 的初始化工作。
 */
void Cycle::bind(  ActionHandle *_ahPtr,
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
    bp->lastIdx = _enterIdx;
    ahPtr->currentIdx = _enterIdx;
    bp->step = _step;
    
    bp->updates = 0;
    bp->step_new = 0;
    bp->is_step_change = false;

    //----- sign up callback funcs -----
    //-- 故意将 首参数this 绑定到 保留类实例 cycle 身上
    
    ahPtr->funcs.insert({ "update", 
            std::bind( &Cycle::update, &cycle, _1 )
            });

    ahPtr->funcs.insert({ "set_step", 
            std::bind( &Cycle::set_step, &cycle, _1, _2 )
            });
    
}


/* ===========================================================
 *                        update
 * -----------------------------------------------------------
 * -- int update( ActionHandle *_ahPtr )
 * -- 需要在 每一视觉帧 调用
 * -- 若 某段时间 不主动调用本函数，动画将陷入停滞（并不会严格对应到全局总时间帧）
 * -- 顺带返回  currentIdx 的值
 */
int Cycle::update( ActionHandle *_ahPtr ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary[0]);

    //=====================================//
    bp->updates++;
    //-------
    int steps = bp->updates / bp->step;
    int newIdx = (bp->lastIdx + steps)%(bp->frames);
    //- 发生了 画面帧 切换 -
    if( ahPtr->currentIdx != newIdx ){
        ahPtr->currentIdx = newIdx;
        if( bp->is_step_change == true ){
            bp->is_step_change = false;
            bp->step = bp->step_new;
            bp->lastIdx = ahPtr->currentIdx;
            bp->updates = 0;
        }
    }

    //=====================================//
    //                ret
    //-------------------------------------//  
    scriptBuf.push_int( ahPtr->currentIdx );
    return sizeof(int);
}


/* ===========================================================
 *                   set_step
 * -----------------------------------------------------------
 * -- void set_step( ActionHandle *_ahPtr, int _step ){
 * -- 重设 step 将清零 updates，并改写 lastIdx。
 * -- 通过不停地 重设 step，可以实现 动画 带有节奏地播放
 * -- 重设 step 不会立即起效，而是先登记下来，等下一 画面帧切换时，再更新 step
 */
int Cycle::set_step( ActionHandle *_ahPtr, int _len ){

    //=====================================//
    //              ptr rebind
    //-------------------------------------//
    assert( _ahPtr->typeId == Cycle::typeId );
    //-- rebind ptr -----
    ahPtr = _ahPtr;
    bp = (Cycle_Binary*)&(ahPtr->binary[0]);
    
    //=====================================//
    //           get params
    //-------------------------------------//
    assert( _len == sizeof(int) ); //- 意义不大
    int _step = scriptBuf.pop_int();

    //=====================================//
    assert( _step > 0 );
    if( _step == bp->step_new ){
        return 0;
    }
    bp->step_new = _step;

    if( bp->is_step_change == true ){
        return 0;
    }else{
        bp->is_step_change = true;
    }

    //=====================================//
    //                ret
    //-------------------------------------// 
    return  0;
}


}//----------------- namespace ActionHdle: end -------------------

