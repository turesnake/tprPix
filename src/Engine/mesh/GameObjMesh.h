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
#ifndef TPR_GAME_OBJ_MESH_H
#define TPR_GAME_OBJ_MESH_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h>  

#include "pch.h"

//-------------------- Engine --------------------//
#include "ChildMesh.h"
#include "AnimAction.h"
#include "AnimActionEName.h"
#include "RotateScaleData.h"
#include "animSubspeciesId.h"
#include "DyBinary.h"
#include "functorTypes.h"


//--- need ---//
class GameObj;
class GameObjMeshSet;

//-- GameObjMesh 是个 "整合类"，不存在进一步的 具象类 --
// GameObjMesh 被轻量化了：
//  -- 不再单独管理自己的 VAO，VBO （改为使用全局唯一的 VAO，VBO）
//  -- texName 存储在 AnimFrameSet 数据中。GameObjMesh 通过 AnimAction 提供的接口访问它们
// --------
//  当切换 action 时，GameObjMesh实例 并不销毁／新建。而是更新自己的 数据组 （空间最优，时间最劣）
//  这个方法也有其他问题：如果不同类型的 go.GameObjMeshs 数量不同，该怎么办？
class GameObjMesh{
    friend class ChildMesh;
    friend class GameObjMeshSet;
public:
    GameObjMesh(    GameObj         &goRef_,
                    const glm::dvec2 &pposOff_,
                    double          zOff_,
                    bool            isVisible_
                    ):
        goRef(goRef_),
        pposOff( tprRound(pposOff_) ), // Must align to pix
        zOff( zOff_),
        isVisible(isVisible_)
        {
            // picMeshUPtr,shadowMeshUPtr 将被延迟到 bind_animAction() 中创建销毁
        }

    void RenderUpdate_auto();
    void RenderUpdate_in_fast_way();

    // 新版，相关参数 由其他 set函数 零散地设置
    // 最后调用 本函数，完成 正式 重绑定工作
    void bind_animAction( int timeStepOff_=0  );

    //----- pvtBinary -----//
    template< typename T >
    inline T *init_pvtBinary()noexcept{
        return this->pvtBinary.init<T>();
    }
    template< typename T >
    inline T *get_pvtBinaryPtr()noexcept{
        return this->pvtBinary.get<T>();
    }

    //------------- set -------------//
    inline void set_animSubspeciesId( animSubspeciesId_t id_ )noexcept{ this->animSubspeciesId = id_; }
    inline void set_animActionEName( AnimActionEName name_ )noexcept{ this->animActionEName = name_; }
    inline void set_alti(double alti_)noexcept{ this->alti = alti_; }
    inline void set_zOff(double zOff_)noexcept{ this->zOff = zOff_; }
    inline void set_uWeight(size_t v_)noexcept{ this->uWeight = v_; }
    inline void set_isNeedToBeErase(bool b_)noexcept{ this->isNeedToBeErase = b_; }
    inline void set_isVisible(bool b_)noexcept{ this->isVisible = b_; }


    inline void accum_alti(double alti_)noexcept{ this->alti += alti_; }
    inline void accum_zOff(double zOff_)noexcept{ this->zOff += zOff_; }
    inline void accum_pposOff( const glm::dvec2 off_ ){ this->pposOff += off_; }

    // speedScale: default=1.0, 值越大，播放速度越缓慢
    inline void bind_reset_playSpeedScale( F_R_double functor_ )noexcept{
        this->animActionPvtData.reset_playSpeedScale = functor_;
    }
    
    //------------- get -------------//    
    inline size_t   get_uWeight()const noexcept{ return this->uWeight; }
    inline bool     get_isNeedToBeErase()const noexcept{ return this->isNeedToBeErase; }
    inline bool     get_isVisible()const noexcept{ return this->isVisible; }


    inline const AnimAction::PvtData *get_animActionPvtDataPtr()const noexcept{ return &(this->animActionPvtData); }

    // 目前仅被 WindAnim 使用
    // 也许未来可以被彻底取消 ...
    inline std::pair<AnimAction::PlayType, AnimAction::PlayState> get_animAction_state()const noexcept{
        return { this->animActionPvtData.playType, this->animActionPvtData.playState };
    }
    
    RotateScaleData rotateScaleData {}; // 管理所有 childMesh rotate 操作

private:
    //------------- set -------------// 
    inline void set_pic_renderLayer( RenderLayerType layerType_ )noexcept{
        this->picRenderLayerType = layerType_;
        if( layerType_ == RenderLayerType::MajorGoes ){
            this->isPicFixedZOff = false;
            this->picBaseZOff = ViewingBox::boxCenter_2_majorGoes;

        }else if( layerType_ == RenderLayerType::BioSoup ){
            this->isPicFixedZOff = false;
            this->picBaseZOff = ViewingBox::boxCenter_2_bioSoup;

        }else{
            this->isPicFixedZOff = true;
            this->picBaseZOff = ViewingBox::get_renderLayerZOff(layerType_);
        }
    }
    inline void set_pic_shader_program( ShaderProgram *sp_ )noexcept{
        tprAssert( this->picMeshUPtr );
        this->picMeshUPtr->set_shader_program( sp_ );
    }
    inline void set_shadow_shader_program( ShaderProgram *sp_ )noexcept{
        tprAssert( this->shadowMeshUPtr );
        this->shadowMeshUPtr->set_shader_program( sp_ );
    }

    //------------- get -------------// 
    inline GLuint get_currentTexName_pic() const noexcept{
        return this->animActionPtr->get_currentTexName_pic( this->animActionPvtData );
    }
    inline GLuint get_currentTexName_shadow() const noexcept{
        tprAssert( this->isHaveShadow );
        return this->animActionPtr->get_currentTexName_shadow( this->animActionPvtData );
    }
    


    //======== vals ========//
    GameObj     &goRef;

    std::unique_ptr<ChildMesh> picMeshUPtr    {nullptr};
    std::unique_ptr<ChildMesh> shadowMeshUPtr {nullptr}; // 不需要时会被及时释放


    // 暂存 gomesh 当前正在播放的 animAction 相关数据
    animSubspeciesId_t  animSubspeciesId {};
    AnimActionEName     animActionEName {}; // 使用 string 很不好 ...


    glm::dvec2  pposOff {}; 
                    //- 以 go.rootAnchor 为 0点的 ppos偏移 
                    //  用来记录，本GameObjMesh 在 go中的 位置（图形）
                    //-- 大部分情况下（尤其是只有一个 GameObjMesh的 go实例），此值为 0
                    //   若本 GameObjMesh实例 是 root GameObjMesh。此值必须为0 (不强制...)  
                    //   此值必须 对齐于 像素

    double      alti {0.0}; 
                    // 其实就是 y轴高度值，只影响 pic
                    // 绝大多数 gomesh，只使用 0.0
                    // 当要表达 类似 烟雾时，可以用上本值

    double      zOff {0.0};   
                    //- 一个 go实例 可能拥有数个 GameObjMesh，相互间需要区分 视觉上的前后顺序
                    //- 此处的 zOff 值只是个 相对偏移值。比如，靠近摄像机的 GameObjMesh zOff +0.1
                    //- 这个值 多数由 具象go类 填入的。
                    // *** 只在 goPic 中有意义，在 shadow 中，应该始终为 0；
                    // 这个值 已经被累加到 z值中.
                    // ----
                    // 如果想要一个浮在所有 MajorGo 前方的图像，可以设置 zOff 为 500.0
                    // 如果想要一个 沉在所有 MajorGo 后方的图像，可设置为 -500.0

    double      picBaseZOff {}; 
                    //- 仅用于 pic-childMesh
                    // 对于 MajorGo / BioSoup，代表 从 取景盒中点，到 各自深度区间的中点 的偏移值 
                    // 对于 其他 rendertype，  代表 从 camera.zFar， 到 各个 rendertype 对应的 固定值

    size_t      uWeight {};
                    // 随机数，方便实现各种功能
                    // 通常在 job线程中 生成后，传递进来
                

    RenderLayerType  picRenderLayerType;

    AnimAction          *animActionPtr {nullptr};
    AnimAction::PvtData  animActionPvtData {}; //- 配合 AnimAction 提供的接口 来使用

    DyBinary   pvtBinary  {}; // store dynamic datas



    //======== flags ========//
    bool    isNeedToBeErase     {false}; // 请求外部管理者，删除自己

    bool    isHaveShadow {}; //- 是否拥有 shadow 数据
                             //- 在 this->init() 之前，此值就被确认了 [被 ChildMesh 使用]
    bool    isVisible  {true};  //- 是否可见 ( go and shadow )    
    bool    isPicFixedZOff {false}; //- 若 renderType 为 MajorGoes / BioSoup, 为 true
                                    // 仅作用于 pic, [被 ChildMesh 使用]


};                           



#endif

