/*
 * ======================= AnimAction.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ANIM_ACTION_H
#define TPR_ANIM_ACTION_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

#include "pch.h"

//-------------------- Engine --------------------//
#include "NineDirection.h"
#include "RGBA.h" 
#include "AnimActionPos.h"
#include "functorTypes.h"
#include "BrokenLvl.h"
#include "AnimActionEName.h"


//-- need --//
class AnimFrameSet;
class AnimActionParam;


//-- 本class 只存储 于 anim-action 有关的 所有静态数据
//   纯粹的静态数据，不允许存储任何与 gomesh相关的动态数据 ！！！！
//   动态数据 存储在 gomesh.animAction::PvtData 中 （每一个 gomesh 独占一份）
class AnimAction{
public:
    //================== nested ================//
    enum class PlayState{
        Stop,   // always for idle, and part of Once
        Working // always for Cycle, and part of Once
    };

    // 动作播放类型 
    enum class PlayType{
        Idle,   //- 从来不更换 动画帧
        Once,   //- 只播放一次，支持 逻辑节点帧，结束后自动跳转到预定的 新状态（未定）
        Cycle   //- 循环播放一段 动画帧
    };
    static PlayType str_2_playType( const std::string &str_ )noexcept;


    // gomesh 自己保存的 有关 animAction 的动态数据。
    class PvtData{
    public:
        size_t   currentIdx_for_frameIdxs {};   //- 当前真正播放的帧，在 frameIdxs 中的下标号
        //---
        size_t   currentFrameIdx {};  //- 当前指向的 frameidx 值
        size_t   currentTimeStep {};  //- 当前帧的 timeStep, （不应被外部访问）
                                    //  被 smoothDeltaTime 修正过
        //---
        size_t   updates {};     //- 切换一次帧后，记录 调用 update() 的次数
        //---
        int      timeStepOff {}; // 外界控制 aaction播放速度的 第一种方式
                                 // 此值会被 累加在每一份 currentTimeStep 上，

        //-- flags --//
        PlayType    playType    {}; // copy from AnimAction.obj
        PlayState   playState   {};
        bool        isLastFrame {false};//- Once: 播放进 最后一帧以及之后，永远处于 true
                                        //- Cycle: 每一次播放到最后一帧，才为 true
                                        //- Idle: always true
                                        //========
                                        // 实际上，由于动画中的每一图元帧，都会被播放好几 时间帧。
                                        // 所有，isLastFrame 并不仅仅在 一个 时间帧 内被设置为 true
                                        // 而是在 尾图元帧 播放的所有 时间帧里，被设置为 true

        //--- functor ---//
        F_R_double  reset_playSpeedScale {nullptr}; // 外界控制 aaction播放速度的 第二种方式
    };


    //================== self ================//
    AnimAction() = default;

    void init(  const AnimFrameSet &animFrameSetRef_,
                const AnimActionParam &param_,
                const AnimActionPos *animActionPosPtr_,
                IntVec2 pixNum_per_frame_,
                size_t headIdx_,
                bool isHaveShadow_ );

    std::function<void(AnimAction::PvtData &)>  update {nullptr};
    
    //- 当 gomesh 切换 animAction 时
    //  通过此函数，来重置自己的 pvtdata 值 --
    inline void reset_pvtData( AnimAction::PvtData &pvtData_ )noexcept{
        pvtData_.currentIdx_for_frameIdxs = 0;
        pvtData_.currentFrameIdx = this->frameIdxs.at(0);
        pvtData_.currentTimeStep = this->timeSteps.at(0);
        pvtData_.playType = this->actionPlayType;
        switch (pvtData_.playType){
            case PlayType::Idle:
                pvtData_.playState = PlayState::Stop; // always
                pvtData_.isLastFrame = true;  // always
                break;
            case PlayType::Once:
                pvtData_.playState = PlayState::Working;
                pvtData_.isLastFrame = false;
                break;
            case PlayType::Cycle:
                pvtData_.playState = PlayState::Working; // always
                pvtData_.isLastFrame = false; 
                break;
            default:
                tprAssert(0);
                break;
        }
    }
    
    //----- get -----//
    inline bool get_isHaveShadow() const noexcept{ return this->isHaveShadow; }
    inline bool get_isOpaque() const noexcept{ return this->isOpaque; }
    inline IntVec2 get_pixNum_per_frame() const noexcept{ return this->pixNum_per_frame; }


    inline PlayType get_actionPlayType()const noexcept{ return this->actionPlayType;  } // 将被废弃

    
    inline const glm::dvec2 &get_currentRootAnchorDPosOff() const noexcept{
        return this->animActionPosPtr->get_rootAnchorDPosOff();
    }

    inline GLuint get_currentTexName_pic( const AnimAction::PvtData &pvtData_ ) const noexcept{
        return this->texNames_pic_ptr->at(pvtData_.currentFrameIdx);
    }
    inline GLuint get_currentTexName_shadow( const AnimAction::PvtData &pvtData_ ) const noexcept{
        return this->texNames_shadow_ptr->at(pvtData_.currentFrameIdx);
    }

    inline const AnimActionPos &get_currentAnimActionPos() const noexcept{
        return *this->animActionPosPtr;
    }


private:
    inline void update_idle( AnimAction::PvtData &pvtData_ ){  }
    void update_once( AnimAction::PvtData &pvtData_ );
    void update_cycle( AnimAction::PvtData &pvtData_ );

    size_t adjust_currentTimeStep( size_t currentTimeStep_, AnimAction::PvtData &pvtData_ );

    //===== vals =====//
    //-- 从 animFrameSet 中获得的 只读指针 --
    const std::vector<GLuint> *texNames_pic_ptr    {nullptr};
    const std::vector<GLuint> *texNames_shadow_ptr {nullptr};

    const AnimActionPos *animActionPosPtr {nullptr}; // 1-animAction, 1-animActionPos 

    PlayType    actionPlayType {}; 

    //- 用户可以手动编排 frameIdx 序列。同时，默认 容器中的第一帧，就是 enterIdx --
    std::vector<size_t> frameIdxs {}; //- 相对于 AnimFrameSet 全frames数据的 idx
    std::vector<size_t> timeSteps {};

    size_t  totalFrameNum {};   //- 本 action 有几帧

    IntVec2  pixNum_per_frame {};  //- 单帧画面 的 长宽 像素值

    //===== flags =====//
    bool   isHaveShadow {};
    bool   isOpaque     {};

};



//-- 作为 AnimFrameSet::insert_a_png() 参数 --
class AnimActionParam{
public:
    //-- 常规构造器,且手动设置 timesteps --
    AnimActionParam(size_t  subspeciesIdx_,
                    AnimActionEName        actionEName_,
                    NineDirection          actionDir_,
                    BrokenLvl              actionBrokenLvl_,
                    AnimAction::PlayType     playType_,
                    bool                   isOrder_,
                    bool                   isOpaque_,
                    size_t                 jFrameIdx_,
                    const std::vector<size_t>  &lFrameIdxs_,
                    const std::vector<size_t>  &timeSteps_,
                    const std::string          &label_ ):
        subspeciesIdx(subspeciesIdx_),
        actionEName(actionEName_),
        actionDir(actionDir_),
        actionBrokenLvl(actionBrokenLvl_),
        actionPlayType( playType_ ),
        isOrder( isOrder_ ),
        isOpaque( isOpaque_ ),
        isTimeStepsManualSet(true),
        jFrameIdx(jFrameIdx_),
        defaultTimeStep(6), //- 随便写一个值，反正用不上
        animLabel(label_)
        {
            this->lFrameIdxs.insert( this->lFrameIdxs.end(), lFrameIdxs_.begin(), lFrameIdxs_.end() );
            this->timeSteps.insert( this->timeSteps.end(), timeSteps_.begin(), timeSteps_.end() );
        }

    //-- 常规构造器,但使用统一值的 timesteps --
    AnimActionParam(size_t  subspeciesIdx_,
                    AnimActionEName       actionEName_,
                    NineDirection         actionDir_,
                    BrokenLvl             actionBrokenLvl_,
                    AnimAction::PlayType    playType_,
                    bool                  isOrder_,
                    bool                  isOpaque_,
                    size_t                jFrameIdx_,
                    const std::vector<size_t>  &lFrameIdxs_,
                    size_t   _defaultTimeStep,
                    const std::string          &label_ ):
        subspeciesIdx(subspeciesIdx_),
        actionEName(actionEName_),
        actionDir(actionDir_),
        actionBrokenLvl(actionBrokenLvl_),
        actionPlayType( playType_ ),
        isOrder( isOrder_ ),
        isOpaque( isOpaque_ ),
        isTimeStepsManualSet(false),
        jFrameIdx(jFrameIdx_),
        defaultTimeStep(_defaultTimeStep),
        animLabel(label_)
        {
            this->lFrameIdxs.insert( this->lFrameIdxs.end(), lFrameIdxs_.begin(), lFrameIdxs_.end() );
            this->timeSteps.push_back( _defaultTimeStep ); //- 用不上
        }

    //-- 单帧action 专用 构造器 --
    AnimActionParam(size_t  subspeciesIdx_,
                    AnimActionEName    actionEName_,
                    NineDirection      actionDir_,
                    BrokenLvl          actionBrokenLvl_,
                    size_t  jFrameIdx_,
                    size_t  lFrameIdx_,
                    bool    isOpaque_,
                    const std::string     &label_ ):
        subspeciesIdx(subspeciesIdx_),
        actionEName(actionEName_),
        actionDir(actionDir_),
        actionBrokenLvl(actionBrokenLvl_),
        actionPlayType( AnimAction::PlayType::Idle ), //- 默认type
        isOrder( true ), //- 随便写一个值，反正用不上
        isOpaque( isOpaque_ ),
        isTimeStepsManualSet(false),
        jFrameIdx(jFrameIdx_),
        defaultTimeStep(6), //- 随便写一个值，反正用不上
        animLabel(label_)
        {
            this->lFrameIdxs.push_back( lFrameIdx_ );
            this->timeSteps.push_back( 6 ); //- 随便写一个值，反正用不上
        }
        
    //===== vals =====//
    size_t          subspeciesIdx;
    AnimActionEName actionEName;
    NineDirection   actionDir;
    BrokenLvl       actionBrokenLvl;
    AnimAction::PlayType  actionPlayType;
    bool            isOrder;
    bool            isOpaque;                //- 是否为 不透明图元
    bool            isTimeStepsManualSet;    //- 若为 false，参数 timeSteps_ 可为空容器
    size_t          jFrameIdx;               //- J帧序号，一个还在发展改善中的数值... 暂时手动设置
    size_t          defaultTimeStep;         //- 若上参数为 false，通过本参数来设置 timeSteps 
    std::vector<size_t> lFrameIdxs {};          //- 和 AnimAction 中的 frameIdxs 不同，此处基于的idx 是相对值
    std::vector<size_t> timeSteps  {}; 
    std::string      animLabel {};
};






#endif 

