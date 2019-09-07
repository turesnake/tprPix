/*
 * ======================= AnimAction.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimAction.h"

//------------------- C --------------------//
#include <cmath>

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "AnimFrameSet.h"
#include "esrc_time.h"


using namespace std::placeholders;


/* ===========================================================
 *                     init
 * -----------------------------------------------------------
 */
void AnimAction::init(  const AnimFrameSet *animFrameSetPtr_,
                        const AnimActionParam &param_,
                        const IntVec2 &pixNum_per_frame_,
                        size_t headIdx_,
                        bool isHaveShadow_ ){

    this->texNames_pic_ptr = animFrameSetPtr_->get_texNames_pic_ptr();
    this->texNames_shadow_ptr = animFrameSetPtr_->get_texNames_shadow_ptr();
    this->framePoses_ptr = animFrameSetPtr_->get_framePoses();
    this->isHaveShadow = isHaveShadow_;
    this->pixNum_per_frame = pixNum_per_frame_;
    this->totalFrameNum = param_.lFrameIdxs.size();
    this->actionType = param_.actionType;
    this->isOpaque = param_.isOpaque;

    //-----------------//
    //    frameIdxs
    //-----------------//
    this->frameIdxs.clear();
    (param_.isOrder) ?
        this->frameIdxs.insert( this->frameIdxs.end(), param_.lFrameIdxs.begin(), param_.lFrameIdxs.end() ) :
        this->frameIdxs.insert( this->frameIdxs.end(), param_.lFrameIdxs.rbegin(), param_.lFrameIdxs.rend() );
    for( auto &ref : this->frameIdxs ){
        ref += headIdx_; //-- 累加 全局起始值
    }

    //-----------------//
    //    timeSteps
    //-----------------//
    this->timeSteps.clear();
    if( param_.isTimeStepsManualSet ){
        tprAssert( param_.timeSteps.size() == param_.lFrameIdxs.size() );
        (param_.isOrder) ?
            this->timeSteps.insert( this->timeSteps.end(), param_.timeSteps.begin(), param_.timeSteps.end() ) :
            this->timeSteps.insert( this->timeSteps.end(), param_.timeSteps.rbegin(), param_.timeSteps.rend() );
    }else{
        this->timeSteps.resize( this->totalFrameNum, param_.defaultTimeStep ); //- 默认值，统统为6
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
            tprAssert(0);
    }
}



/* ===========================================================
 *                     update_once
 * -----------------------------------------------------------
 * 将给定的帧序列 播放一次。当到达最后一帧时，改写 pvtData_.isLastFrame
 * 在正常流程中，外部代码在接受到这个 状态值 后，会根据具体情况，切换新的 action
 * 如果外部代码未作为，本函数将继续播放 最后一帧.
 */
void AnimAction::update_once( AnimActionPvtData &pvtData_ ){

    //-- 无限停留在最后一帧，并一直返回 LastFrame 
    if( pvtData_.isLastFrame == true ){
        return;
    }

    pvtData_.updates++;
    //----- node frame -----//
    if( pvtData_.updates >= pvtData_.currentTimeStep ){
        pvtData_.updates = 0;
        
        //--- currentIdx_for_frameIdxs ---
        if(pvtData_.currentIdx_for_frameIdxs==this->totalFrameNum-1){
            pvtData_.isLastFrame = true;
            return;
        }
    
        pvtData_.currentIdx_for_frameIdxs++;
        pvtData_.currentFrameIdx = this->frameIdxs.at( pvtData_.currentIdx_for_frameIdxs );
        pvtData_.currentTimeStep = AnimAction::adjust_currentTimeStep_by_smoothDeltaTime(
                                        this->timeSteps.at(pvtData_.currentIdx_for_frameIdxs) );
    }
}


/* ===========================================================
 *                     update_cycle
 * -----------------------------------------------------------
 * 无限循环 给定的帧序列，永不停止，除非被外部 切换成其他 action
 */
void AnimAction::update_cycle( AnimActionPvtData &pvtData_ ){

    pvtData_.updates++;
    //----- node frame -----//
    if( pvtData_.updates >= pvtData_.currentTimeStep ){
        pvtData_.updates = 0;
        
        //--- currentIdx_for_frameIdxs ---
        (pvtData_.currentIdx_for_frameIdxs==this->totalFrameNum-1) ? //- end 
                pvtData_.currentIdx_for_frameIdxs=0 :
                pvtData_.currentIdx_for_frameIdxs++;

        pvtData_.currentFrameIdx = this->frameIdxs.at( pvtData_.currentIdx_for_frameIdxs );
        pvtData_.currentTimeStep = AnimAction::adjust_currentTimeStep_by_smoothDeltaTime(
                                    this->timeSteps.at(pvtData_.currentIdx_for_frameIdxs) );
    }
}



/* ===========================================================
 *    adjust_currentTimeStep_by_smoothDeltaTime   [static]
 * -----------------------------------------------------------
 * 目前默认，excel数据中记录的 timeSteps，以 60pfs 为基准
 * return:
 *     调整过的 currentTimeStep
 */
size_t AnimAction::adjust_currentTimeStep_by_smoothDeltaTime( size_t currentTimeStep_ ){

    double smoothDeltaTime = esrc::get_timer().get_smoothDeltaTime();
    constexpr double mid = (1.0 / 60.0) * 5.0;
    return cast_2_size_t( floor( mid / smoothDeltaTime ) );
}
