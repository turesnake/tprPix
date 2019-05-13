/*
 * ======================= AnimAction.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimAction.h"

//-------------------- C --------------------//
#include <cassert>

//------------------- Engine --------------------//
#include "AnimFrameSet.h"


using namespace std::placeholders;


/* ===========================================================
 *                     init
 * -----------------------------------------------------------
 */
void AnimAction::init(  const AnimFrameSet *_animFrameSetPtr,
                        const AnimActionParam &_param,
                        const IntVec2 &_pixNum_per_frame,
                        size_t _headIdx,
                        bool _isHaveShadow ){

    this->texNames_pic_ptr = _animFrameSetPtr->get_texNames_pic_ptr();
    this->texNames_shadow_ptr = _animFrameSetPtr->get_texNames_shadow_ptr();
    this->framePosesPtr = _animFrameSetPtr->get_framePosesPtr();


    this->isHaveShadow = _isHaveShadow;
    this->pixNum_per_frame = _pixNum_per_frame;

    this->totalFrameNum = _param.lFrameIdxs.size();
    
    this->actionType = _param.actionType;

    //-----------------//
    //    frameIdxs
    //-----------------//
    this->frameIdxs.clear();
    (_param.isOrder) ?
        this->frameIdxs.insert( this->frameIdxs.end(), _param.lFrameIdxs.begin(), _param.lFrameIdxs.end() ) :
        this->frameIdxs.insert( this->frameIdxs.end(), _param.lFrameIdxs.rbegin(), _param.lFrameIdxs.rend() );
    for( auto &ref : this->frameIdxs ){
        ref += _headIdx; //-- 累加 全局起始值
    }

    //-----------------//
    //    timeSteps
    //-----------------//
    this->timeSteps.clear();
    if( _param.isTimeStepsManualSet ){
        assert( _param.timeSteps.size() == _param.lFrameIdxs.size() );
        (_param.isOrder) ?
            this->timeSteps.insert( this->timeSteps.end(), _param.timeSteps.begin(), _param.timeSteps.end() ) :
            this->timeSteps.insert( this->timeSteps.end(), _param.timeSteps.rbegin(), _param.timeSteps.rend() );
    }else{
        this->timeSteps.resize( this->totalFrameNum, _param.defaultTimeStep ); //- 默认值，统统为6
    } 

    //---------------------//
    //    update functor
    //---------------------//
    switch( this->actionType ){
        case AnimActionType::Idle:
            this->update = std::bind( &AnimAction::update_idle, this, _1 );
            break;
        case AnimActionType::Once:
            this->update = std::bind( &AnimAction::update_once, this, _1 );
            break;
        case AnimActionType::Cycle:
            this->update = std::bind( &AnimAction::update_cycle, this, _1 );
            break;
        default:
            assert(0);
    }
}



/* ===========================================================
 *                     update_once
 * -----------------------------------------------------------
 * 将给定的帧序列 播放一次。当到达最后一帧时，改写 _pvtData.isLastFrame
 * 在正常流程中，外部代码在接受到这个 状态值 后，会根据具体情况，切换新的 action
 * 如果外部代码未作为，本函数将继续播放 最后一帧.
 */
void AnimAction::update_once( AnimActionPvtData &_pvtData ){

    //-- 无限停留在最后一帧，并一直返回 LastFrame 
    if( _pvtData.isLastFrame == true ){
        return;
    }

    _pvtData.updates++;
    //----- node frame -----//
    if( _pvtData.updates >= _pvtData.currentTimeStep ){
        _pvtData.updates = 0;
        
        //--- currentIdx_for_frameIdxs ---
        if(_pvtData.currentIdx_for_frameIdxs==this->totalFrameNum-1){
            _pvtData.isLastFrame = true;
            return;
        }
    
        _pvtData.currentIdx_for_frameIdxs++;
        _pvtData.currentFrameIdx = this->frameIdxs.at( _pvtData.currentIdx_for_frameIdxs );
        _pvtData.currentTimeStep = this->timeSteps.at( _pvtData.currentIdx_for_frameIdxs );
    }
}


/* ===========================================================
 *                     update_cycle
 * -----------------------------------------------------------
 * 无限循环 给定的帧序列，永不停止，除非被外部 切换成其他 action
 */
void AnimAction::update_cycle( AnimActionPvtData &_pvtData ){

    _pvtData.updates++;
    //----- node frame -----//
    if( _pvtData.updates >= _pvtData.currentTimeStep ){
        _pvtData.updates = 0;
        
        //--- currentIdx_for_frameIdxs ---
        (_pvtData.currentIdx_for_frameIdxs==this->totalFrameNum-1) ? //- end 
                _pvtData.currentIdx_for_frameIdxs=0 :
                _pvtData.currentIdx_for_frameIdxs++;

        _pvtData.currentFrameIdx = this->frameIdxs.at( _pvtData.currentIdx_for_frameIdxs );
        _pvtData.currentTimeStep = this->timeSteps.at( _pvtData.currentIdx_for_frameIdxs );
    }
}


