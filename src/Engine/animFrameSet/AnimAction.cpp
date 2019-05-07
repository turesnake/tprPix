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
    this->isOrder = _param.isOrder;

    this->frameIdxs.clear();
    this->timeSteps.clear();

    //-- frameIdx 需要加上 起始值 --
    for( const auto &i : _param.lFrameIdxs ){
        this->frameIdxs.push_back( _headIdx + i );
    }

    //--
    if( _param.isTimeStepsManualSet ){
        assert( _param.timeSteps.size() == _param.lFrameIdxs.size() );
        this->timeSteps.insert( this->timeSteps.end(), _param.timeSteps.begin(), _param.timeSteps.end() );
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
 *                     update_cycle
 * -----------------------------------------------------------
 */
void AnimAction::update_cycle( AnimActionPvtData &_pvtData ){

    _pvtData.updates++;
    //----- node frame -----//
    if( _pvtData.updates >= _pvtData.currentTimeStep ){
        _pvtData.updates = 0;
        
        //--- currentIdx_for_frameIdxs ---
        if( this->isOrder == true ){
            (_pvtData.currentIdx_for_frameIdxs==this->totalFrameNum-1) ? //- end 
                    _pvtData.currentIdx_for_frameIdxs=0 :
                    _pvtData.currentIdx_for_frameIdxs++;
        }else{
            (_pvtData.currentIdx_for_frameIdxs==0) ?  //- beg
                    _pvtData.currentIdx_for_frameIdxs=this->totalFrameNum-1 :
                    _pvtData.currentIdx_for_frameIdxs--;
        }

        _pvtData.currentFrameIdx = this->frameIdxs.at( _pvtData.currentIdx_for_frameIdxs );
        _pvtData.currentTimeStep = this->timeSteps.at( _pvtData.currentIdx_for_frameIdxs );
    }
}











