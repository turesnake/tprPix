/*
 * ========================= Cycle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    动画动作 处理器：简单死循环动作 处理器 [第二版]
 * ----------------------------
 */
#ifndef _TPR_ANIM_FRAME_IDX_HANDLE_CYCLE_H_
#define _TPR_ANIM_FRAME_IDX_HANDLE_CYCLE_H_

//-------------------- C ----------------------//
#include <cassert> //-- assert

//-------------------- CPP ----------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "ScriptBuf.h"
#include "AnimFrameIdxHandle.h"


namespace animFrameIdxHdle{//------------- namespace animFrameIdxHdle ----------------


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
    bool isOrder;      //- 正序播放(true) ／ 倒序播放(false)
    //------ padding -----
    u8   padding[2];
};


//- 适用于： 一个无限循环的动画 action --
//  从入口帧开始，周而复始地播放。
//  当外部发生事件，需要退出 本 animFrameSet 时。会立即退出（从任何一帧）
class Cycle{
public:
    //-- 仅用于 本ah实例 初始化阶段 --
    void bind(  AnimFrameIdxHandle *_ahPtr,
                int _begIdx,
                int _endIdx,
                int _enterIdx,
                const std::vector<int> &_steps,
                bool _isStepEqual,
                bool _isOrder );

    //-- 仅用于 本ah实例 绑定新的 action 阶段 --
    void rebind(  AnimFrameIdxHandle *_ahPtr,
                int _begIdx,
                int _endIdx,
                int _enterIdx,
                const std::vector<int> &_steps,
                bool _isStepEqual,
                bool _isOrder );

    //------ tmp ptr ------
    // 不能信赖，务必在每次 callback 时重新绑定
    AnimFrameIdxHandle  *ahPtr  {nullptr}; //- 
    Cycle_Binary  *bp     {nullptr}; //- 指向 ahPtr->binary

    //------ static vals -------
    static u32  typeId;

    //------- callback --------//  
    //-- 为了提高运行效率，制作了两个版本
    int update( AnimFrameIdxHandle *_ahPtr );
};

//=====< Cycle类 唯一的 保留实例 >=====
inline Cycle  cycle_obj {};


}//----------------- namespace animFrameIdxHdle: end -------------------
#endif 

