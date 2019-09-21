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
//#include "Collision.h" 
#include "ChildMesh.h"
#include "AnimAction.h"
#include "RotateScaleData.h"
#include "animSubspeciesId.h"


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
    explicit GameObjMesh( GameObj &goRef_ ):
        goRef(goRef_)
        {
            // picMeshUPtr,shadowMeshUPtr 将被延迟到 bind_animAction() 中创建销毁
        }

    void RenderUpdate_auto();

    void bind_animAction(   animSubspeciesId_t subspeciesId_,
                            NineDirection      dir_,
                            const std::string &actionName_  );

    //------------- set -------------//
    inline void set_pic_renderLayer( RenderLayerType layerType_ )noexcept{
        this->picRenderLayerType = layerType_;
        if( layerType_ == RenderLayerType::MajorGoes ){
            this->isPicFixedZOff = false;
            this->picFixedZOff = 0.0; //- null
        }else{
            this->isPicFixedZOff = true;
            this->picFixedZOff = static_cast<float>(ViewingBox::get_renderLayerZOff(layerType_));
        }
    }
    inline void set_pposOff( const glm::vec2 &pposOff_ )noexcept{
        this->pposOff = pposOff_;
    }
    inline void set_off_z( float off_z_ )noexcept{
        this->off_z = off_z_;
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
    inline const AnimActionPos &get_currentAnimActionPos() const noexcept{
        return this->animActionPtr->get_currentAnimActionPos();
    }   

    inline const GLuint &get_currentTexName_pic() const noexcept{
        return this->animActionPtr->get_currentTexName_pic( this->animActionPvtData );
    }
    inline const GLuint &get_currentTexName_shadow() const noexcept{
        tprAssert( this->isHaveShadow );
        return this->animActionPtr->get_currentTexName_shadow( this->animActionPvtData );
    }
    inline const glm::dvec2 &get_currentRootAnchorDPosOff() const noexcept{
        return this->animActionPtr->get_currentRootAnchorDPosOff();
    }

   inline const IntVec2 &get_animAction_pixNum_per_frame() const noexcept{
        return this->animActionPtr->get_pixNum_per_frame();
    }
    inline const glm::vec2 &get_pposOff() const noexcept{
        return this->pposOff;
    }
    inline const float &get_off_z() const noexcept{
        return this->off_z;
    }
    inline const float &get_picFixedZOff() const noexcept{
        return this->picFixedZOff;
    }

    //-- 当播放 once 类型动作时，外部代码，通过此函数，来判断，是否播放到最后一帧 --
    inline bool get_isLastFrame() const noexcept{
        return this->animActionPvtData.isLastFrame;
    }

    inline const GameObj &get_goCRef() const noexcept{
        return this->goRef;
    }

    RotateScaleData rotateScaleData {}; // 管理所有 childMesh rotate 操作

    //======== flags ========//
    bool    isHaveShadow {}; //- 是否拥有 shadow 数据
                            //- 在 this->init() 之前，此值就被确认了 [被 ChildMesh 使用]
    bool    isVisible  {true};  //- 是否可见 ( go and shadow )    
    //bool    isCollide  {true};  //- 本mesh所拥有的 碰撞区 是否参与 碰撞检测
    //bool    isFlipOver {false}; //- 图形左右翻转： false==不翻==向右； true==翻==向左；
                                // -- gmesh.isFlipOver 决定了 此图元的 静态方向
                                // -- go.isFlipOver    决定了 此图元 的动态方向，比如走动时
    bool    isPicFixedZOff {false}; //- 是否使用 用户设置的 固定 zOff 值
                                    // 仅作用于 pic, [被 ChildMesh 使用]

private:
    //======== vals ========//
    GameObj      &goRef;

    std::unique_ptr<ChildMesh> picMeshUPtr    {nullptr};
    std::unique_ptr<ChildMesh> shadowMeshUPtr {nullptr}; // 不需要时会被及时释放

    glm::vec2  pposOff {}; //- 以 go.rootAnchor 为 0点的 ppos偏移 
                    //  用来记录，本GameObjMesh 在 go中的 位置（图形）
                    //-- 大部分情况下（尤其是只有一个 GameObjMesh的 go实例），此值为 0
                    //   若本 GameObjMesh实例 是 root GameObjMesh。此值必须为0

    float      off_z {0.0f};   //- 一个 go实例 可能拥有数个 GameObjMesh，相互间需要区分 视觉上的前后顺序
                    //- 此处的 off_z 值只是个 相对偏移值。比如，靠近摄像机的 GameObjMesh off_z +0.1f
                    //- 这个值 多数由 具象go类 填入的。
                    // *** 只在 goPic 中有意义，在 shadow 中，应该始终为 0；
                    // 这个值 已经被累加到 z值中.
                    // ----
                    // 如果想要一个浮在所有 MajorGo 前方的图像，可以设置 off_z 为 500.0
                    // 如果想要一个 沉在所有 MajorGo 后方的图像，可设置为 -500.0

    float            picFixedZOff {}; //- 方便快速访问
    RenderLayerType  picRenderLayerType;

    AnimAction        *animActionPtr {nullptr};
    AnimActionPvtData  animActionPvtData {}; //- 配合 AnimAction 提供的接口 来使用
};



#endif

