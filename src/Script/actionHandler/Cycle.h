/*
 * ========================= Cycle.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器
 * ----------------------------
 */
#ifndef _TPR_ACTION_HANDLER_CYCLE_H_
#define _TPR_ACTION_HANDLER_CYCLE_H_

//-------------------- C ----------------------//
#include <cassert> //-- assert

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Self --------------------//
#include "ActionHandler.h"


namespace actionHdlr{//------------- namespace ActionHdlr ----------------


struct Cycle_Binary{
    int  frames;     //- action实例 的 总画面帧数
    int  enterIdx;   //- 入口帧序号. 这个值永不变
    int  lastIdx;    //-当 step值被重设时，这个值会被对齐到 当时的 currentIdx
    int  currentIdx; //- 当前指向的 画面帧序号（基于0）
    int  step;       //- 每切换一帧画面，停留多少 时间帧
    //----------------
    int  updates;    //- 本实例自创建以来，调用 update() 的次数
    int  step_new;       //- 临时存储 新的 step值
    bool is_step_change; //- step 是否被要求更换
};


//- 适用于： 一个无限循环的动画action --
//  从入口帧开始，周而复始地播放。
//  当外部发生事件，需要退出 本action 时。会立即退出（从任何一帧）
class Cycle{
public:

    void bind(  ActionHandler *_ahPtr,
                int _frames,
                int _enterIdx,
                int _step );

    //------ tmp ptr ------
    // 不能信赖，务必在每次 callback 时重新绑定
    ActionHandler  *ahPtr  {nullptr}; //- 
    Cycle_Binary   *bp     {nullptr}; //- 指向 ahPtr->binary

    //------ static vals -------
    static u32  typeId;

    //------- callback --------//  
    void *update( ActionHandler *_ahPtr );
    void *get_currentIdx( ActionHandler *_ahPtr );
    void *set_step( ActionHandler *_ahPtr, void *_buf, int *_len );


};

//=====< Cycle类 唯一的 保留实例 >=====
inline Cycle  ah_cycle {};




}//----------------- namespace ActionHdlr: end -------------------
#endif 

