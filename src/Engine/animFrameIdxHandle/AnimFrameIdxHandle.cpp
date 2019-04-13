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
    this->goMeshPtr = _goMeshPtr;
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

    this->type = AnimFrameIdxHandleType::Cycle;
    //-------
    assert( (_enterIdx>=_begIdx) && (_enterIdx<=_endIdx) );
    this->cycleData.begIdx = _begIdx;
    this->cycleData.endIdx = _endIdx;
    this->cycleData.enterIdx = _enterIdx;
    this->cycleData.updates = 0;
    this->cycleData.currentTimeStep = this->goMeshPtr->get_animFrameSet_currentTimeStep( _enterIdx );
    this->cycleData.isOrder = _isOrder;
    //-------
    this->currentIdx = _enterIdx;
}


/* ===========================================================
 *                     bind_once
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::bind_once(){
    this->type = AnimFrameIdxHandleType::Once;
    //-------
    //... 尚未实现 ...
}



/* ===========================================================
 *                   update_cycle
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::update_cycle(){

    this->cycleData.updates++;
    //----- node frame -----//
    if( this->cycleData.updates >= this->cycleData.currentTimeStep ){
        this->cycleData.updates = 0;
        
        //--- currentIdx ---
        if( this->cycleData.isOrder == true ){
            (this->currentIdx==this->cycleData.endIdx) ?  
                    this->currentIdx=this->cycleData.begIdx :
                    this->currentIdx++;
        }else{
            (this->currentIdx==this->cycleData.begIdx) ?  
                    this->currentIdx=this->cycleData.endIdx :
                    this->currentIdx--;
        }

        //--- currentTimeStep ---
        this->cycleData.currentTimeStep = this->goMeshPtr->get_animFrameSet_currentTimeStep( this->currentIdx );
    }
}


/* ===========================================================
 *                   update_once
 * -----------------------------------------------------------
 */
void AnimFrameIdxHandle::update_once(){

    //... 尚未实现 ...
}

