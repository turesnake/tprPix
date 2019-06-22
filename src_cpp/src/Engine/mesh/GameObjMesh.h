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
#ifndef TPR_GAME_MESH_H_
#define TPR_GAME_MESH_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h>  

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Collision.h" 
#include "ChildMesh.h"
#include "AnimAction.h"


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
    GameObjMesh() = default;

    inline void init( GameObj *_goPtr ){
        this->goPtr = _goPtr;
        //-----
        this->picMesh.init(    this->goPtr, const_cast<GameObjMesh*>(this) );
        this->shadowMesh.init( this->goPtr, const_cast<GameObjMesh*>(this) ); //- 就算没有 shadow，也会执行 init
    }

    void RenderUpdate();

    void bind_animAction(   const std::string &_animFrameSetName,
                            const std::string &_actionName  );

    //------------- set -------------//
    inline void set_pic_renderLayer( RenderLayerType _layerType ){
        this->picRenderLayerType = _layerType;
        if( _layerType == RenderLayerType::MajorGoes ){
            this->isPicFixedZOff = false;
            this->picFixedZOff = 0.0; //- null
        }else{
            this->isPicFixedZOff = true;
            this->picFixedZOff = ViewingBox::get_renderLayerZOff(_layerType);
        }
    }
    inline void set_pposOff( const glm::vec2 &_pposOff ){
        this->pposOff = _pposOff;
    }
    inline void set_off_z( float _off_z ){
        this->off_z = _off_z;
    }
    inline void set_pic_shader_program( ShaderProgram *_sp ){
        this->picMesh.set_shader_program( _sp );
    }
    inline void set_shadow_shader_program( ShaderProgram *_sp ){
        this->shadowMesh.set_shader_program( _sp );
    }


    //------------- get -------------//    
    inline const FramePos &get_currentFramePos() const {
        return this->animActionPtr->get_currentFramePos( this->animActionPvtData );
    }   
    inline const GLuint &get_currentTexName_pic() const {
        return this->animActionPtr->get_currentTexName_pic( this->animActionPvtData );
    }
    inline const GLuint &get_currentTexName_shadow() const {
        tprAssert( this->isHaveShadow );
        return this->animActionPtr->get_currentTexName_shadow( this->animActionPvtData );
    }
    inline const IntVec2 &get_currentRootAnchorPPosOff() const {
        return this->animActionPtr->get_currentRootAnchorPPosOff( this->animActionPvtData );
    }
   inline const IntVec2 &get_animAction_pixNum_per_frame() const {
        return this->animActionPtr->get_pixNum_per_frame();
    }
    inline const glm::vec2 &get_pposOff() const {
        return this->pposOff;
    }
    inline const float &get_off_z() const {
        return this->off_z;
    }
    inline const float &get_picFixedZOff() const {
        return this->picFixedZOff;
    }

    //-- 当播放 once 类型动作时，外部代码，通过此函数，来判断，是否播放到最后一帧 --
    inline bool get_isLastFrame() const {
        return this->animActionPvtData.isLastFrame;
    }


    //======== flags ========//
    bool    isHaveShadow {}; //- 是否拥有 shadow 数据
                            //- 在 this->init() 之前，此值就被确认了 [被 ChildMesh 使用]
    bool    isVisible  {true};  //- 是否可见 ( go and shadow )    
    bool    isCollide  {true};  //- 本mesh所拥有的 碰撞区 是否参与 碰撞检测
    bool    isFlipOver {false}; //- 图形左右翻转： false==不翻==向右； true==翻==向左；
                                // -- gmesh.isFlipOver 决定了 此图元的 静态方向
                                // -- go.isFlipOver    决定了 此图元 的动态方向，比如走动时
    bool    isPicFixedZOff {false}; //- 是否使用 用户设置的 固定 zOff 值
                                    // 仅作用于 pic, [被 ChildMesh 使用]
private:
    //======== vals ========//
    GameObj    *goPtr {nullptr}; //- 每个 GameObjMesh实例 都属于一个 go实例. 强关联

    ChildMesh   picMesh    { true };
    ChildMesh   shadowMesh { false }; //- 当某个 gomesh实例 没有 shadow时，此数据会被空置

    glm::vec2  pposOff {}; //- 以 go.rootAnchor 为 0点的 ppos偏移 
                    //  用来记录，本GameObjMesh 在 go中的 位置（图形）
                    //-- 大部分情况下（尤其是只有一个 GameObjMesh的 go实例），此值为 0
                    //   若本 GameObjMesh实例 是 root GameObjMesh。此值必须为0

    float      off_z {0.0f};   //- 一个 go实例 可能拥有数个 GameObjMesh，相互间需要区分 视觉上的前后顺序
                    //- 此处的 off_z 值只是个 相对偏移值。比如，靠近摄像机的 GameObjMesh off_z +0.1f
                    //- 这个值 多数由 具象go类 填入的。
                    // *** 只在 goPic 中有意义，在 shadow 中，应该始终为 0；

    float            picFixedZOff {}; //- 方便快速访问
    RenderLayerType  picRenderLayerType;

    AnimAction        *animActionPtr {nullptr};
    AnimActionPvtData  animActionPvtData {}; //- 配合 AnimAction 提供的接口 来使用
};



#endif

