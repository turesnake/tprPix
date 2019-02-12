/*
 * ================== AnimFrameIdxHandle.cpp ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    动画帧 idx 管理器   [第二版]
 *    ------
 *    管理器内部分成数个 子类型，可通过 flags 来切换
 * ----------------------------
 */
#include "AnimFrameIdxHandle.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- Engine --------------------//
#include "GameObjMesh.h"


/* ===========================================================
 *                     init
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::init( GameObjMesh *_goMeshPtr ){
    goMeshPtr = _goMeshPtr;
    //....
}


/* ===========================================================
 *                     bind_cycle
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::bind_cycle(int _begIdx,
                                    int _endIdx,
                                    int _enterIdx,
                                    bool _isOrder ){

    type = AnimFrameIdxHandleType::Cycle;
    //-------
    assert( (_enterIdx>=_begIdx) && (_enterIdx<=_endIdx) );
    cycleData.begIdx = _begIdx;
    cycleData.endIdx = _endIdx;
    cycleData.enterIdx = _enterIdx;
    cycleData.updates = 0;
    cycleData.currentTimeStep = goMeshPtr->get_animFrameSet_currentTimeStep( _enterIdx );
    cycleData.isOrder = _isOrder;

    currentIdx = _enterIdx;
}


/* ===========================================================
 *                     bind_once
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::bind_once(){
    type = AnimFrameIdxHandleType::Once;
    //-------
    //...
}





/* ===========================================================
 *                   update_cycle
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::update_cycle(){

    cycleData.updates++;

    //----- node frame -----//
    if( cycleData.updates >= cycleData.currentTimeStep ){
        cycleData.updates = 0;
        
        //--- currentIdx ---
        if( cycleData.isOrder == true ){
            (currentIdx==cycleData.endIdx) ? currentIdx=cycleData.begIdx :
                                             currentIdx++;
        }else{
            (currentIdx==cycleData.begIdx) ? currentIdx=cycleData.endIdx :
                                             currentIdx--;
        }

        //--- currentTimeStep ---
        cycleData.currentTimeStep = goMeshPtr->get_animFrameSet_currentTimeStep( currentIdx );
    }
}


/* ===========================================================
 *                   update_once
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::update_once(){



}

