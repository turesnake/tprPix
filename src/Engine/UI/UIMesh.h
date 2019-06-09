/*
 * ========================= UIMesh.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  UIObj 专用 Mesh， 类似 GO 的 GoMesh
 * ----------------------------
 */
#ifndef TPR_UI_MESH_H_
#define TPR_UI_MESH_H_
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
#include "UIChildMesh.h"
#include "AnimAction.h"

//--- need ---//
class UIObj;

class AnimFrameSet;


class UIMesh{
public:
    UIMesh() = default;

    inline void init( UIObj *_uiGoPtr ){
        this->uiGoPtr = _uiGoPtr;
        //-----
        this->picMesh.init(    this->uiGoPtr, const_cast<UIMesh*>(this) );
        this->shadowMesh.init( this->uiGoPtr, const_cast<UIMesh*>(this) ); //- 就算没有 shadow，也会执行 init
    }

    void RenderUpdate();

    void bind_animAction(   const std::string &_animFrameSetName,
                            const std::string &_actionName  );

    //------------- set -------------//
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
    inline const IntVec2 &get_currentRootAnchorPPosOff() const {
        return this->animActionPtr->get_currentRootAnchorPPosOff( this->animActionPvtData );
    }
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
    inline const IntVec2 &get_animAction_pixNum_per_frame() const {
        return this->animActionPtr->get_pixNum_per_frame();
                        //-- 这个函数会在每帧被调用。最好的措施是，每次 切换 animaction 时，将这个数据，暂存到 gomesh 中。
    }
    inline const float &get_off_z() const {
        return this->off_z;
    }

    //======== flags ========//
    bool    isHaveShadow {}; //- 是否拥有 shadow 数据，在 bind_animFrameSet() 中配置.
                            //- 在 this->init() 之前，此值就被确认了 [被 ChildMesh 使用]
    bool    isVisible  {true};  //- 是否可见 ( go and shadow )    
    bool    isFlipOver {false}; //- 图形左右翻转： false==不翻==向右； true==翻==向左；
                                // -- gmesh.isFlipOver 决定了 此图元的 静态方向
                                // -- go.isFlipOver    决定了 此图元 的动态方向，比如走动时

private:
    //======== vals ========//
    UIObj  *uiGoPtr {nullptr}; //- 每个 UIMesh实例 都属于一个 go实例. 强关联

    UIChildMesh   picMesh    { true };
    UIChildMesh   shadowMesh { false }; //- 当某个 gomesh实例 没有 shadow时，此数据会被空置

    float      off_z {0.0f};   //- 一个 go实例 可能拥有数个 GameObjMesh，相互间需要区分 视觉上的前后顺序
                    //- 此处的 off_z 值只是个 相对偏移值。比如，靠近摄像机的 GameObjMesh off_z +0.1f
                    //- 这个值 多数由 具象go类 填入的。
                    // *** 只在 goPic 中有意义，在 shadow 中，应该始终为 0；
    
    AnimAction        *animActionPtr {nullptr};
    AnimActionPvtData  animActionPvtData {}; //- 配合 AnimAction 提供的接口 来使用
};



#endif

