/*
 * ========================= Cycle.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器
 * ----------------------------
 */
#ifndef _TPR_ACTION_HANDLE_CYCLE_H_
#define _TPR_ACTION_HANDLE_CYCLE_H_

//-------------------- C ----------------------//
#include <cassert> //-- assert

//-------------------- CPP ----------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "ScriptBuf.h"
#include "ActionHandle.h"


namespace actionHdle{//------------- namespace actionHdle ----------------


struct Cycle_Binary{

    std::vector<int>  steps; //- 此vector实例 自身占有 24-bytes。
                             //- 并将 此struct 对齐标准提升到 8-bytes

    int  begIdx;      //- 循环起始帧序号. 这个值永不变
    int  endIdx;      //- 循环结束帧序号. 这个值永不变
    int  enterIdx;     //- 指定从那一帧开始播 （不一定是 begIdx）

    //----------------
    int  updates;    //- 每次帧切换后，用此值来记录 调用 update() 的次数

    int  currentStep;  //- 当前帧 使用的 step
    bool isStepEqual;  //- 使用单一step值: steps.at(0); 还是使用 steps 离散值
    //------ padding -----
    u8   padding[3];
};


//- 适用于： 一个无限循环的动画action --
//  从入口帧开始，周而复始地播放。
//  当外部发生事件，需要退出 本action 时。会立即退出（从任何一帧）
class Cycle{
public:
    void bind(  ActionHandle *_ahPtr,
                int _begIdx,
                int _endIdx,
                int _enterIdx,
                const std::vector<int> &_steps,
                bool _isStepEqual );

    //------ tmp ptr ------
    // 不能信赖，务必在每次 callback 时重新绑定
    ActionHandle  *ahPtr  {nullptr}; //- 
    Cycle_Binary  *bp     {nullptr}; //- 指向 ahPtr->binary

    //------ static vals -------
    static u32  typeId;

    //------- callback --------//  
    int update( ActionHandle *_ahPtr );
};

//=====< Cycle类 唯一的 保留实例 >=====
inline Cycle  cycle_obj {};


}//----------------- namespace actionHdle: end -------------------
#endif 

