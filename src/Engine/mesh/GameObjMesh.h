/*
 * ========================= GameObjMesh.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   A Mesh class bind to GameObj & AnimFrameSet.
 *  ----------
 * GameObjMesh 和 AnimFrameSet 的区别：
 *	  GameObjMesh 需要 图片资源，
 *	  而大部分 图片资源，以 gl texName 的形式，存储在 AnimFrameSet 模块中。
 *  ----------
 *   一个 GameObjMesh 实例 拥有：
 *     一个 根锚点 ／ root anchor   -- 代表 GameObjMesh 本身
 *     数个 子锚点 ／ child anchor  -- 用来绑定其他 GameObjMeshes
 *  ----------
 *   和 go类一样，GameObjMesh 只记录数据，具体的使用方式，由最终的 具象go类 区定义
 * ----------------------------
 */
#ifndef _TPR_GAME_MESH_H_
#define _TPR_GAME_MESH_H_
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
#include "Collision.h" 
#include "AnchorPos.h"
#include "ChildMesh.h"

//--- need ---//
class GameObj;

//-- GameObjMesh 是个 "整合类"，不存在进一步的 具象类 --
// GameObjMesh 被轻量化了：
//  -- 不再单独管理自己的 VAO，VBO （改为使用全局唯一的 VAO，VBO）
//  -- texName 存储在 AnimFrameSet 数据中。GameObjMesh 通过一个指针 调用它们
//    （调用方式由 具象go类 定义）
// --------
// 每个 具象go类实例 所拥有的 GameObjMesh实例，都将被单独存储 在 mem态。（比如存储在 具象go实例 内）
// GameObjMesh实例 不会被存入 硬盘态。
// 而是在每次 加载section中 具象go实例时，临时生成。
// --------
//  当切换 action 时，GameObjMesh实例 并不销毁／新建。而是更新自己的 数据组 （空间最优，时间最劣）
//  这个方法也有其他问题：如果不同类型的 go.GameObjMeshs 数量不同，该怎么办？
// --------
// GameObjMesh实例拥有：
//  -1- animFrameSetPtr.  实例本体 存储在 全局容器 animFrameSets 中。
//  -2- animFrameIdxHandle. 实例（独占）
//  -3- 2 个 子mesh实例， pic / shadow
//      有些 gomesh 可能没有 shadow，此时为空置   
//
class GameObjMesh{
public:
    GameObjMesh() = default;

    inline void init( GameObj *_goPtr ){
        goPtr = _goPtr;
        //-----
        this->picMesh.init(    goPtr, const_cast<GameObjMesh*>(this) );
        this->shadowMesh.init( goPtr, const_cast<GameObjMesh*>(this) ); //- 就算没有 shadow，也会执行 init
    }

    void RenderUpdate();

    

    //------ animFrameSet ------
    //  目前，此函数仅用于 go.creat_new_goMesh() 中
    //  以及 切换动画时
    void bind_animFrameSet( const std::string &_name );

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
        return this->animFrameSetPtr->get_framePoses().at( this->animFrameIdxHandle.currentIdx ).get_rootAnchorPos();
    }
    inline const FramePos &get_currentFramePos() const {
        return this->animFrameSetPtr->get_framePoses().at( this->animFrameIdxHandle.currentIdx );
    }
    inline const GLuint &get_currentTexName_pic() const {
        return this->animFrameSetPtr->get_texNames_pic().at(this->animFrameIdxHandle.currentIdx);
    }
    inline const GLuint &get_currentTexName_shadow() const {
        return this->animFrameSetPtr->get_texNames_shadow().at(this->animFrameIdxHandle.currentIdx);
    }
    inline const IntVec2 &get_currentRootAnchorPPosOff() const {
        return this->animFrameSetPtr->get_framePoses().at(this->animFrameIdxHandle.currentIdx).get_rootAnchorPos().pposOff;
    }
    inline const IntVec2 &get_animFrameSet_pixNum_per_frame() const {
        return this->animFrameSetPtr->get_pixNum_per_frame();
    }
    inline const int &get_animFrameSet_currentTimeStep( int _currentIdx ) const {
        return this->animFrameSetPtr->get_timeSteps().at(_currentIdx);
    }
    inline const glm::vec2 &get_pposOff() const {
        return this->pposOff;
    }
    inline const float &get_off_z() const {
        return this->off_z;
    }
    inline AnimFrameIdxHandle &getnc_animFrameIdxHandle(){
        return this->animFrameIdxHandle;
    }
    inline const float &get_picFixedZOff() const {
        return this->picFixedZOff;
    }

    //======== flags ========//
    bool    isHaveShadow {}; //- 是否拥有 shadow 数据，在 bind_animFrameSet() 中配置.
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
    GameObj  *goPtr {nullptr}; //- 每个 GameObjMesh实例 都属于一个 go实例. 强关联

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

    AnimFrameIdxHandle  animFrameIdxHandle {}; //- 一个 GameObjMesh拥有一个 ah实例
                                    //- 由于 ah实例 只存在于mem态，所以几乎很少存在 反射的需求。
                                    //- 但是存在 类型验证的需求：通过 .typeId 

    float            picFixedZOff {}; //- 方便快速访问
    RenderLayerType  picRenderLayerType;

    //------- AnimFrameSet -------
    // 具象go类代码 通过 name／id 来 设置／改写 AnimFrameSet 数据
    // 在单一时间，一个GameObjMesh实例 只能绑定 一个 animFrameSetPtr实例
    std::string     animFrameSetName;       //- 与下方的 animFrameSetPtr 强关联 --
    AnimFrameSet   *animFrameSetPtr {nullptr}; //- 指向 esrc::animFrameSets 中的某个 AnimFrameSet 实例
                                    //- 通过 bind_animFrameSet() 来绑定


};



#endif

