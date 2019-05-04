/*
 * ================== AnimFrameIdxHandle.h ===================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    动画帧 idx 管理器   [第二版]
 *    ------
 *    管理器内部分成数个 子类型，可通过 flags 来切换
 * ----------------------------
 */
#ifndef _TPR_ANIM_FRAME_IDX_HANDLE_H_
#define _TPR_ANIM_FRAME_IDX_HANDLE_H_

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>
#include <unordered_map> 

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "ScriptBuf.h"

//-- 支持的几种 子类型 --
enum class AnimFrameIdxHandleType{
    Idle,  //- 从来不更换 动画帧
    Cycle, //- 循环播放一段 动画帧
    Once   //- 只播放一次，支持 逻辑节点帧，结束后自动跳转到预定的 新状态（未定）
};


struct CycleData{
    int  begIdx;      //- 循环起始帧序号. 这个值永不变
    int  endIdx;      //- 循环结束帧序号. 这个值永不变
    int  enterIdx;     //- 指定从那一帧开始播 （不一定是 begIdx）
    //----------------
    int  updates;    //- 每次帧切换后，用此值来记录 调用 update() 的次数
    int  currentTimeStep;  //- 当前帧的 timeStep, 
    bool isOrder;      //- 正序播放(true) ／ 倒序播放(false)
};


struct OnceData{
    int  begIdx;      //- 循环起始帧序号. 这个值永不变, 同时也是 enterIdx
    int  endIdx;      //- 循环结束帧序号. 这个值永不变
    //----------------
    int  updates;    //- 每次帧切换后，用此值来记录 调用 update() 的次数
};


//-- need --//
class GameObjMesh;
class UIMesh;

//-- 
class AnimFrameIdxHandle{
    using F_1 = std::function<const int&(int)>;
public:
    AnimFrameIdxHandle() = default;

    inline void bind_get_animFrameSet_currentTimeStep_func( const F_1 &_func ){
        this->get_animFrameSet_currentTimeStep_func = _func;
    }


    inline void bind_idle( int _idx ){
        this->type = AnimFrameIdxHandleType::Idle;
        this->currentIdx = _idx; //- never change
    }

    void bind_cycle(int _begIdx,
                    int _endIdx,
                    int _enterIdx,
                    bool _isOrder );

    void bind_once();

    //- return: 此帧 currentIdx 是否发生切换
    //  如果没切换，gomesh.renderupdate 中可以省掉很多操作 
    inline void update(){
        switch( this->type ){
        case AnimFrameIdxHandleType::Idle:
            //-- do nothing --
            break;
        case AnimFrameIdxHandleType::Cycle:
            update_cycle();
            break;
        case AnimFrameIdxHandleType::Once:
            update_once();
            break;
        default:
            assert(0);
        }
    }

    //----------- common vals -------------//
    int                     currentIdx; //- 当前指向的 画面帧序号（基于0）
    AnimFrameIdxHandleType  type { AnimFrameIdxHandleType::Cycle };

    //-- child data --
    CycleData   cycleData {};
    OnceData    onceData  {};


private:
    void update_cycle();
    void update_once();

    //===== vals =====//
    F_1  get_animFrameSet_currentTimeStep_func {nullptr};
            //-- 通过这个 functor，使得本模块，可以兼容 goMesh, uiMesh
            //-- 这是一个很好的策略。
                
};


#endif

