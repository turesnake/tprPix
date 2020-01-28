/*
 * ========================= GameObjMesh.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   A Mesh class bind to GameObj & AnimAction.
 *  ----------
 * GameObjMesh 和 AnimFrameSet 的区别：
 *	  GameObjMesh 需要 图片资源，
 *	  而大部分 图片资源，以 gl texName 的形式，存储在 AnimFrameSet 模块中。
 *  ----------
 *   一个 GameObjMesh 实例 拥有：
 *     一个 根锚点 ／ root anchor   -- 代表 GameObjMesh 本身
 *     数个 子锚点 ／ child anchor  -- 用来绑定其他 GameObjMeshes
 * ----------------------------
 */
#ifndef TPR_GAME_MESH_H
#define TPR_GAME_MESH_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h>  

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <string>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprMath.h"
#include "ChildMesh.h"
#include "AnimAction.h"
#include "AnimActionEName.h"
#include "RotateScaleData.h"
#include "animSubspeciesId.h"
#include "RenderPool.h"
#include "colorTableId.h"
#include "DyBinary.h"
#include "functorTypes.h"


//--- need ---//
class GameObj;

//-- GameObjMesh 是个 "整合类"，不存在进一步的 具象类 --
// GameObjMesh 被轻量化了：
//  -- 不再单独管理自己的 VAO，VBO （改为使用全局唯一的 VAO，VBO）
//  -- texName 存储在 AnimFrameSet 数据中。GameObjMesh 通过 AnimAction 提供的接口访问它们
// --------
//  当切换 action 时，GameObjMesh实例 并不销毁／新建。而是更新自己的 数据组 （空间最优，时间最劣）
//  这个方法也有其他问题：如果不同类型的 go.GameObjMeshs 数量不同，该怎么办？
class GameObjMesh{
public:
    explicit GameObjMesh(   GameObj         &goRef_,
                            const glm::dvec2 &pposOff_,
                            double          zOff_,
                            bool            isVisible_
                             ):
        isVisible(isVisible_),
        goRef(goRef_),
        pposOff( tprRound(pposOff_) ), // Must align to pix
        zOff( zOff_)
        {
            // picMeshUPtr,shadowMeshUPtr 将被延迟到 bind_animAction() 中创建销毁
        }

    void RenderUpdate_auto();
    void RenderUpdate_ground();

    //--------------------------//
    //   bind_animAction 系列函数，暂未设计完善

    // 旧版，显式输入 所有相关参数
    /*
    void bind_animAction(   animSubspeciesId_t    subspeciesId_,
                            NineDirection      dir_,
                            BrokenLvl          brokenLvl_,
                            const std::string &actionName_,
                            int                timeStepOff_=0  );
    */

    // 新版，相关参数 由其他 set函数 零散地设置
    // 最后调用 本函数，完成 正式 重绑定工作
    void bind_animAction( int timeStepOff_=0  );


    //------------- set -------------//
    inline void set_pic_renderLayer( RenderLayerType layerType_ )noexcept{
        this->picRenderLayerType = layerType_;
        if( layerType_ == RenderLayerType::MajorGoes ){
            this->isPicFixedZOff = false;
            this->picFixedZOff = 0.0; //- null
        }else{
            this->isPicFixedZOff = true;
            this->picFixedZOff = ViewingBox::get_renderLayerZOff(layerType_);
        }
    }
    inline void set_colorTableId( colorTableId_t id_ )noexcept{ this->colorTableId = id_; }
    inline void set_animSubspeciesId( animSubspeciesId_t id_ )noexcept{ this->animSubspeciesId = id_; }
    inline void set_animActionEName( AnimActionEName name_ )noexcept{ this->animActionEName = name_; }


    inline void set_pic_shader_program( ShaderProgram *sp_ )noexcept{
        tprAssert( this->picMeshUPtr );
        this->picMeshUPtr->set_shader_program( sp_ );
    }
    inline void set_shadow_shader_program( ShaderProgram *sp_ )noexcept{
        tprAssert( this->shadowMeshUPtr );
        this->shadowMeshUPtr->set_shader_program( sp_ );
    }
    inline void bind_reset_playSpeedScale( F_R_double functor_ )noexcept{
        this->animActionPvtData.reset_playSpeedScale = functor_;
    }
    inline void bind_pic_before_drawCall( F_void f_ ){
        tprAssert( this->picMeshUPtr );
        this->picMeshUPtr->bind_before_drawCall( f_ );
    }

    //------------- get -------------//    
    inline const AnimActionPos &get_currentAnimActionPos() const noexcept{
        return this->animActionPtr->get_currentAnimActionPos();
    }   
    inline GLuint get_currentTexName_pic() const noexcept{
        return this->animActionPtr->get_currentTexName_pic( this->animActionPvtData );
    }
    inline GLuint get_currentTexName_shadow() const noexcept{
        tprAssert( this->isHaveShadow );
        return this->animActionPtr->get_currentTexName_shadow( this->animActionPvtData );
    }
    inline const glm::dvec2 &get_currentRootAnchorDPosOff() const noexcept{
        return this->animActionPtr->get_currentRootAnchorDPosOff();
    }

    inline IntVec2 get_animAction_pixNum_per_frame() const noexcept{
        return this->animActionPtr->get_pixNum_per_frame();
    }
    inline const glm::dvec2 &get_pposOff()const noexcept{ return this->pposOff; }
    inline double get_zOff()const noexcept{ return this->zOff; }
    inline double get_picFixedZOff() const noexcept{ return this->picFixedZOff; }
    inline const GameObj &get_goCRef() const noexcept{ return this->goRef; }
    inline DyBinary &get_pvtBinary()noexcept{ return this->pvtBinary; }



    inline std::pair<AnimAction::PlayType, AnimAction::State> get_animAction_state()const noexcept{
        auto type = this->animActionPtr->get_actionPlayType();
        switch (type){
            case AnimAction::PlayType::Idle:  return { type, AnimAction::State::Stop };
            case AnimAction::PlayType::Cycle: return { type, AnimAction::State::Working };
            case AnimAction::PlayType::Once:
                return ((this->animActionPvtData.isLastFrame) ?
                        std::pair<AnimAction::PlayType, AnimAction::State>{ type, AnimAction::State::Stop } :
                        std::pair<AnimAction::PlayType, AnimAction::State>{ type, AnimAction::State::Working });
            default:
                tprAssert(0);
                return { AnimAction::PlayType::Idle, AnimAction::State::Stop }; // never reach
        }
    }

    
    RotateScaleData rotateScaleData {}; // 管理所有 childMesh rotate 操作

    //======== flags ========//
    bool    isHaveShadow {}; //- 是否拥有 shadow 数据
                             //- 在 this->init() 之前，此值就被确认了 [被 ChildMesh 使用]
    bool    isVisible  {true};  //- 是否可见 ( go and shadow )    
    bool    isPicFixedZOff {false}; //- 是否使用 用户设置的 固定 zOff 值
                                    // 仅作用于 pic, [被 ChildMesh 使用]
    //bool    isRegularVAOVBO {true}; // 绝大多数 gomesh 选择 true，使用一个 rect，配合 tex来显示图元
                                    // groundGo meshs 除外，它们使用 triangleFan 不绑定 tex
                                    // 会影响 childMesh 的行为

private:
    //======== vals ========//
    GameObj    &goRef;

    std::unique_ptr<ChildMesh> picMeshUPtr    {nullptr};
    std::unique_ptr<ChildMesh> shadowMeshUPtr {nullptr}; // 不需要时会被及时释放


    // 暂存 gomesh 当前正在播放的 animAction 相关数据
    animSubspeciesId_t  animSubspeciesId {};
    AnimActionEName     animActionEName {}; // 使用 string 很不好 ...


    glm::dvec2  pposOff {}; //- 以 go.rootAnchor 为 0点的 ppos偏移 
                    //  用来记录，本GameObjMesh 在 go中的 位置（图形）
                    //-- 大部分情况下（尤其是只有一个 GameObjMesh的 go实例），此值为 0
                    //   若本 GameObjMesh实例 是 root GameObjMesh。此值必须为0 (不强制...)  
                    //   此值必须 对齐于 像素

    double     zOff {0.0};   //- 一个 go实例 可能拥有数个 GameObjMesh，相互间需要区分 视觉上的前后顺序
                    //- 此处的 zOff 值只是个 相对偏移值。比如，靠近摄像机的 GameObjMesh zOff +0.1
                    //- 这个值 多数由 具象go类 填入的。
                    // *** 只在 goPic 中有意义，在 shadow 中，应该始终为 0；
                    // 这个值 已经被累加到 z值中.
                    // ----
                    // 如果想要一个浮在所有 MajorGo 前方的图像，可以设置 zOff 为 500.0
                    // 如果想要一个 沉在所有 MajorGo 后方的图像，可设置为 -500.0

    double            picFixedZOff {}; //- 方便快速访问
    RenderLayerType  picRenderLayerType;

    AnimAction          *animActionPtr {nullptr};
    AnimAction::PvtData  animActionPvtData {}; //- 配合 AnimAction 提供的接口 来使用

    colorTableId_t  colorTableId { MaxColorTableId }; // just used in GroundGo 临时而又丑陋的实现 ...

    DyBinary   pvtBinary  {}; // store dynamic datas
};                           



#endif

