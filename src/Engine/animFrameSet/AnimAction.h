/*
 * ======================= AnimAction.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_ANIM_ACTION_H_
#define _TPR_ANIM_ACTION_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>


//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "RGBA.h" 
#include "FramePos.h"


//-- 动作类型 --
enum class AnimActionType{
    Idle,   //- 从来不更换 动画帧
    Once,   //- 只播放一次，支持 逻辑节点帧，结束后自动跳转到预定的 新状态（未定）
    Cycle   //- 循环播放一段 动画帧
};



//-- gomesh 自己保存的 有关 animAction 的动态数据。
class AnimActionPvtData{
public:
    size_t   currentIdx_for_frameIdxs {};   //- 当前真正播放的帧，在 frameIdxs 中的下标号
    //---
    size_t   currentFrameIdx {};  //- 当前指向的 frameidx 值
    size_t   currentTimeStep {};  //- 当前帧的 timeStep, （不应被外部访问）
    //---
    size_t   updates {};     //- 切换一次帧后，记录 调用 update() 的次数
    //-- flags --//
    bool     isLastFrame {false}; //- 仅用于 Once 模式
};


//-- 作为 AnimFrameSet::insert_a_png() 参数 --
class AnimActionParam{
public:

    //-- 常规构造器,且手动设置 timesteps --
    AnimActionParam(const std::string  &_actionName,
                    const AnimActionType &_type,
                    bool                _isOrder,
                    const std::vector<size_t>  &_lFrameIdxs,
                    const std::vector<size_t>  &_timeSteps ):
        actionName(_actionName),
        actionType( _type ),
        isOrder( _isOrder ),
        isTimeStepsManualSet(true),
        defaultTimeStep(6) //- 随便写一个值，反正用不上
        {
            this->lFrameIdxs.insert( this->lFrameIdxs.end(), _lFrameIdxs.begin(), _lFrameIdxs.end() );
            this->timeSteps.insert( this->timeSteps.end(), _timeSteps.begin(), _timeSteps.end() );
        }

    //-- 常规构造器,但使用统一值的 timesteps --
    AnimActionParam(const std::string  &_actionName,
                    const AnimActionType &_type,
                    bool                _isOrder,
                    const std::vector<size_t>  &_lFrameIdxs,
                    size_t   _defaultTimeStep ):
        actionName(_actionName),
        actionType( _type ),
        isOrder( _isOrder ),
        isTimeStepsManualSet(false),
        defaultTimeStep(_defaultTimeStep)
        {
            this->lFrameIdxs.insert( this->lFrameIdxs.end(), _lFrameIdxs.begin(), _lFrameIdxs.end() );
            this->timeSteps.push_back( _defaultTimeStep ); //- 用不上
        }

    //-- 单帧action 专用 构造器 --
    AnimActionParam(    const std::string &_actionName,
                        size_t  _lFrameIdx ):
        actionName(_actionName),
        actionType( AnimActionType::Idle ), //- 默认type
        isOrder( true ), //- 随便写一个值，反正用不上
        isTimeStepsManualSet(false),
        defaultTimeStep(6) //- 随便写一个值，反正用不上
        {
            this->lFrameIdxs.push_back( _lFrameIdx );
            this->timeSteps.push_back( 6 ); //- 随便写一个值，反正用不上
        }

    //===== vals =====//
    std::string     actionName;
    AnimActionType  actionType;
    bool            isOrder;
    bool            isTimeStepsManualSet;    //- 若为 false，参数 _timeSteps 可为空容器
    size_t          defaultTimeStep;         //- 若上参数为 false，通过本参数来设置 timeSteps 
    std::vector<size_t> lFrameIdxs;          //- 和 AnimAction 中的 frameIdxs 不同，此处基于的idx 是相对值
    std::vector<size_t> timeSteps; 
};


//-- need --//
class AnimFrameSet;


//-- 本class 只存储 于 anim-action 有关的 所有静态数据
//   动态数据 存储在 gomesh.animActionPvtData 中 （每一个 gomesh 独占一份）
class AnimAction{
    using F_UPDATE = std::function<void(AnimActionPvtData &)>;

public:
    AnimAction() = default;


    void init(  const AnimFrameSet *_animFrameSetPtr,
                const AnimActionParam &_param,
                const IntVec2 &_pixNum_per_frame,
                size_t _headIdx,
                bool _isHaveShadow );

    F_UPDATE  update {nullptr};


    //- 当 gomesh 切换 animAction 时
    //  通过此函数，来重置自己的 pvtdata 值 --
    inline void reset_pvtData( AnimActionPvtData &_pvtData ){
        _pvtData.currentIdx_for_frameIdxs = 0;
        _pvtData.currentFrameIdx = this->frameIdxs.at(0);
        _pvtData.currentTimeStep = this->timeSteps.at(0);
        _pvtData.isLastFrame = false;
    }


    //----- get -----//
    inline bool get_isHaveShadow() const {
        return this->isHaveShadow;
    }
    inline const IntVec2 &get_pixNum_per_frame() const {
        return this->pixNum_per_frame;
    }
    inline const IntVec2 &get_currentRootAnchorPPosOff( const AnimActionPvtData &_pvtData ) const {
        return this->framePosesPtr->at(_pvtData.currentFrameIdx).get_rootAnchorPPosOff();
    }
    inline const GLuint &get_currentTexName_pic( const AnimActionPvtData &_pvtData ) const {
        return this->texNames_pic_ptr->at(_pvtData.currentFrameIdx);
    }
    inline const GLuint &get_currentTexName_shadow( const AnimActionPvtData &_pvtData ) const {
        return this->texNames_shadow_ptr->at(_pvtData.currentFrameIdx);
    }
    inline const FramePos &get_currentFramePos( const AnimActionPvtData &_pvtData ) const {
        return this->framePosesPtr->at(_pvtData.currentFrameIdx);
    }

private:

    inline void update_idle( AnimActionPvtData &_pvtData ){}
    void update_once( AnimActionPvtData &_pvtData );
    void update_cycle( AnimActionPvtData &_pvtData );

    //===== vals =====//

    //-- 从 animFrameSet 中获得的 只读指针 --
    const std::vector<GLuint> *texNames_pic_ptr {nullptr};
    const std::vector<GLuint> *texNames_shadow_ptr {nullptr};
    const std::vector<FramePos> *framePosesPtr {nullptr};

    AnimActionType   actionType {}; 

    //- 用户可以手动编排 frameIdx 序列。同时，默认 容器中的第一帧，就是 enterIdx --
    std::vector<size_t> frameIdxs; //- 相对于 AnimFrameSet 全frames数据的 idx
    std::vector<size_t> timeSteps;

    size_t  totalFrameNum {};   //- 本 action 有几帧

    IntVec2  pixNum_per_frame {};  //- 单帧画面 的 长宽 像素值

    //===== flags =====//
    bool   isHaveShadow;

};


#endif 

