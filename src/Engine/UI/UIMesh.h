/*
 * ========================= UIMesh.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#ifndef _TPR_UI_MESH_H_
#define _TPR_UI_MESH_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h>  

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "AnimFrameIdxHandle.h"
#include "AnimFrameSet.h" 
#include "AnchorPos.h"
#include "UIChildMesh.h"

//--- need ---//
class UIObj;


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

    //------ animFrameSet ------
    //  目前，此函数仅用于 go.creat_new_goMesh() 中
    //  以及 切换动画时
    void bind_animFrameSet( const std::string &_name );

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
    inline const std::string &get_animFrameSetName() const {
        return this->animFrameSetName;
    }
    inline const int &get_totalFrames() const {
        return this->animFrameSetPtr->get_totalFrameNum();
    }
    inline const int &get_currentAnimFrameIdx() const { //--- IMPORTANT !!! ---
        return this->animFrameIdxHandle.currentIdx;
    }
    inline const AnchorPos &get_rootAnchorPos() const {
        return this->animFrameSetPtr->get_framePos( this->animFrameIdxHandle.currentIdx ).get_rootAnchorPos();
    }
    inline const FramePos &get_currentFramePos() const {
        return this->animFrameSetPtr->get_framePos( this->animFrameIdxHandle.currentIdx );
    }
    inline const GLuint &get_currentTexName_pic() const {
        return this->animFrameSetPtr->get_texName_pic( this->animFrameIdxHandle.currentIdx );
    }
    inline const GLuint &get_currentTexName_shadow() const {
        return this->animFrameSetPtr->get_texName_shadow( this->animFrameIdxHandle.currentIdx );
    }
    inline const IntVec2 &get_currentRootAnchorPPosOff() const {
        return this->animFrameSetPtr->get_framePos( this->animFrameIdxHandle.currentIdx ).get_rootAnchorPos().pposOff;
    }
    inline const IntVec2 &get_animFrameSet_pixNum_per_frame() const {
        return this->animFrameSetPtr->get_pixNum_per_frame();
    }
    inline const int &get_animFrameSet_currentTimeStep( int _currentIdx ) const {
        return this->animFrameSetPtr->get_timeStep( _currentIdx );
    }

    inline const float &get_off_z() const {
        return this->off_z;
    }
    inline AnimFrameIdxHandle &getnc_animFrameIdxHandle(){
        return this->animFrameIdxHandle;
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

    AnimFrameIdxHandle  animFrameIdxHandle {}; //- 一个 GameObjMesh拥有一个 ah实例
                                    //- 由于 ah实例 只存在于mem态，所以几乎很少存在 反射的需求。
                                    //- 但是存在 类型验证的需求：通过 .typeId 


    //------- AnimFrameSet -------
    // 具象go类代码 通过 name／id 来 设置／改写 AnimFrameSet 数据
    // 在单一时间，一个GameObjMesh实例 只能绑定 一个 animFrameSetPtr实例
    std::string     animFrameSetName;       //- 与下方的 animFrameSetPtr 强关联 --
    AnimFrameSet   *animFrameSetPtr {nullptr}; //- 指向 esrc::animFrameSets 中的某个 AnimFrameSet 实例
                                    //- 通过 bind_animFrameSet() 来绑定
};



#endif

