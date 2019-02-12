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


enum class AnimFrameIdxHandleType{
    Cycle,
    Once
};


struct Cycle_Data{
    int  begIdx;      //- 循环起始帧序号. 这个值永不变
    int  endIdx;      //- 循环结束帧序号. 这个值永不变
    int  enterIdx;     //- 指定从那一帧开始播 （不一定是 begIdx）

    //----------------
    int  updates;    //- 每次帧切换后，用此值来记录 调用 update() 的次数

    int  currentTimeStep;  //- 当前帧的 timeStep, 
    bool isOrder;      //- 正序播放(true) ／ 倒序播放(false)
    //------ padding -----
    //u8   padding[3]; //- 无需对齐
};


struct Once_Data{
    int  begIdx;      //- 循环起始帧序号. 这个值永不变, 同时也是 enterIdx
    int  endIdx;      //- 循环结束帧序号. 这个值永不变
    //----------------
    int  updates;    //- 每次帧切换后，用此值来记录 调用 update() 的次数
};



//-- need --//
class GameObjMesh;

//-- 
class AnimFrameIdxHandle{
public:
    AnimFrameIdxHandle() = default;

    void init( GameObjMesh *_goMeshPtr );

    void bind_cycle(int _begIdx,
                    int _endIdx,
                    int _enterIdx,
                    bool _isOrder );


    void bind_once();


    inline void update(){
        switch( type ){
        case AnimFrameIdxHandleType::Cycle:
            update_cycle();
            break;
        case AnimFrameIdxHandleType::Once:
            update_once();
            break;
        default:
            assert(0);
            break; //- never touch
        }
    }

    //----------- common vals -------------//
    int  currentIdx; //- 当前指向的 画面帧序号（基于0）

    AnimFrameIdxHandleType  type { AnimFrameIdxHandleType::Cycle };


    Cycle_Data   cycleData {};
    Once_Data    onceData  {};


private:
    GameObjMesh *goMeshPtr {nullptr};

    void update_cycle();
    void update_once();

};


#endif

