/*
 * ======================= AnimAction.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "AnimAction.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"

//------------------- Engine --------------------//
#include "AnimFrameSet.h"

#include "esrc_time.h"


using namespace std::placeholders;



AnimAction::PlayType AnimAction::str_2_playType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<AnimAction::PlayType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        tprDebug::console( "can't find AnimAction::PlayType: {}", str_  );
        tprAssert(0);
        return AnimAction::PlayType::Idle; // never reach
    }
}



void AnimAction::init(  const AnimFrameSet &animFrameSetRef_,
                        const AnimActionParam &param_,
                        const AnimActionPos *animActionPosPtr_,
                        IntVec2 pixNum_per_frame_,
                        size_t headIdx_,
                        bool isHaveShadow_ ){

    this->texNames_pic_ptr = animFrameSetRef_.get_texNames_pic_ptr();
    this->texNames_shadow_ptr = animFrameSetRef_.get_texNames_shadow_ptr();
    this->animActionPosPtr = animActionPosPtr_;
    this->isHaveShadow = isHaveShadow_;
    this->pixNum_per_frame = pixNum_per_frame_;
    this->totalFrameNum = param_.lFrameIdxs.size();
    this->actionPlayType = param_.actionPlayType;
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
    switch( this->actionPlayType  ){
        case PlayType::Idle:
            this->update = std::bind( &AnimAction::update_idle, this, _1 );
            break;
        case PlayType::Once:
            this->update = std::bind( &AnimAction::update_once, this, _1 );
            break;
        case PlayType::Cycle:
            this->update = std::bind( &AnimAction::update_cycle, this, _1 );
            break;
        default:
            tprAssert(0);
    }
}




// 将给定的帧序列 播放一次。当到达最后一帧时，改写 pvtData_.isLastFrame
// 在正常流程中，外部代码在接受到这个 状态值 后，会根据具体情况，切换新的 action
// 如果外部代码未作为，本函数将继续播放 最后一帧.
void AnimAction::update_once( AnimAction::PvtData &pvtData_ ){

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
            pvtData_.playState = PlayState::Stop;
            pvtData_.isLastFrame = true;
            return;
        }
    
        pvtData_.currentIdx_for_frameIdxs++;
        pvtData_.currentFrameIdx = this->frameIdxs.at( pvtData_.currentIdx_for_frameIdxs );
        pvtData_.currentTimeStep = this->adjust_currentTimeStep(
                                        this->timeSteps.at(pvtData_.currentIdx_for_frameIdxs), pvtData_ );
    }
}



// 无限循环 给定的帧序列，永不停止，除非被外部 切换成其他 action
void AnimAction::update_cycle( AnimAction::PvtData &pvtData_ ){

    pvtData_.updates++;
    //----- node frame -----//
    if( pvtData_.updates >= pvtData_.currentTimeStep ){
        pvtData_.updates = 0;
        
        //--- currentIdx_for_frameIdxs ---
        if( pvtData_.currentIdx_for_frameIdxs == this->totalFrameNum-1 ){
            // last frame
            pvtData_.currentIdx_for_frameIdxs=0;
            if( pvtData_.isLastFrame != true ){
                pvtData_.isLastFrame = true;
            }
        }else{
            pvtData_.currentIdx_for_frameIdxs++;
            if( pvtData_.isLastFrame != false ){
                pvtData_.isLastFrame = false;
            }
        }

        pvtData_.currentFrameIdx = this->frameIdxs.at( pvtData_.currentIdx_for_frameIdxs );
        pvtData_.currentTimeStep = this->adjust_currentTimeStep(
                                    this->timeSteps.at(pvtData_.currentIdx_for_frameIdxs), pvtData_ );                                    
    }
}



// 目前默认，json 数据中记录的 timeSteps，以 60pfs 为基准
// return:
//     调整过的 currentTimeStep 
// -------
// 目前实现了 2 种方法来控制 动画播放速度：
// -1- reset_playSpeedScale(); 播放倍率
//     适用于，原本 afs数据中，每一张画面，就要播放 4～7帧 的缓慢动画，比如风吹草动动画 
//     但它不适合处理，原本就是 一帧一张图的 快速动画
// -2- timeStepOff
//     适合处理 一帧一张图 的快速动画，效果没有 方法1 细腻 和 直观。
//
//      也许在未来会被统一为一个 控制器。目前先这么混合用着 
size_t AnimAction::adjust_currentTimeStep( size_t currentTimeStep_, AnimAction::PvtData &pvtData_ ){

    //--------------//
    //  修正帧率波动
    //--------------//
    double playSpeedScale = (pvtData_.reset_playSpeedScale==nullptr) ? 1.0 : pvtData_.reset_playSpeedScale();
    double v = (1.0 / 60.0) * static_cast<double>(currentTimeStep_) * playSpeedScale;
    double smoothDeltaTime = esrc::get_timer().get_smoothDeltaTime();
    int step = static_cast<int>( floor( v / smoothDeltaTime ) );
    if( step < 1 ){ step = 1; }

    //--------------//
    // 累加 timeStepOff
    //--------------//
    step += pvtData_.timeStepOff;

    //--------------//
    // limit: [1, 100]
    //--------------//
    if( step < 1 ){     step = 1; }
    if( step > 100 ){   step = 100; }

    return cast_2_size_t(step);
}

