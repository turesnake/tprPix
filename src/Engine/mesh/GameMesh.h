/*
 * ========================= GameMesh.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   A Mesh class bind to GameObj & Action.
 *  ----------
 * GameMesh 和 Action 的区别：
 *	  GameMesh 需要 图片资源，
 *	  而大部分 图片资源，以 gl texName 的形式，存储在 Action 模块中。
 *  ----------
 *   一个 GameMesh 实例 拥有：
 *     一个 根锚点 ／ root anchor   -- 代表 GameMesh 本身
 *     数个 子锚点 ／ child anchor  -- 用来绑定其他 GameMeshes
 *  ----------
 *   和 go类一样，GameMesh 只记录数据，具体的使用方式，由最终的 具象go类 区定义
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
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "ActionHandle.h"
#include "Action.h" 
#include "Collision.h" 
#include "AnchorPos.h"
#include "ChildMesh.h"

//--- need ---//
class GameObj;

//-- GameMesh 是个 "整合类"，不存在它的 具象类 --
// GameMesh 被轻量化了：
//  -- 不再单独管理自己的 VAO，VBO （改为使用全局唯一的 VAO，VBO）
//  -- texName 存储在 action数据中。GameMesh 通过一个指针 调用它们
//    （调用方式由 具象go类 定义）
// --------
// 每个 具象go类实例 所拥有的 GameMesh实例，都将被单独存储 在 mem态。（比如存储在 具象go实例 内）
// GameMesh实例 不会被存入 硬盘态。
// 而是在每次 加载section中 具象go实例时，临时生成。
// --------
//  当切换 action时，gameMesh实例 并不销毁／新建。而是更新自己的 数据组 （空间最优，时间最劣）
//  这个方法也有其他问题：如果不同类型的 go.gameMeshs 数量不同，该怎么办？
// --------
// GameMesh实例拥有：
//  -1- actionPtr.  实例本体 存储在 全局容器 actions 中。
//  -2- actionHandle. 实例（独占）
//  GameMesh 整合这两个数据，并实现最终的 draw() 函数 
//
class GameMesh{
public:
    GameMesh() = default;

    inline void init( GameObj *_goPtr ){
        goPtr = _goPtr;
        //-----
        picMesh.init(    goPtr, (GameMesh*)this );
        shadowMesh.init( goPtr, (GameMesh*)this );
    }

    //------ action ------
    void bind_action( const std::string &_name );

    inline const std::string &get_actionName() const {
        return actionName;
    }

    inline int get_totalFrames() const {
        return actionPtr->totalFrameNum;
    }

    //--- IMPORTANT !!! ---
    inline int get_currentActionFrameIdx() const {
        return actionHandle.currentIdx;
    }

    //--- IMPORTANT !!! ---
    inline const AnchorPos &get_rootAnchorPos() const {
        return actionPtr->framePoses.at( actionHandle.currentIdx ).get_rootAnchorPos();
    }

    //--- IMPORTANT !!! ---
    inline const FramePos &get_currentFramePos() const {
        return actionPtr->framePoses.at( actionHandle.currentIdx );
    }

    inline GLuint get_currentTexName_pic() const {
        return actionPtr->texNames_pic.at(actionHandle.currentIdx);
    }
    inline GLuint get_currentTexName_shadow() const {
        return actionPtr->texNames_shadow.at(actionHandle.currentIdx);
    }

    inline const IntVec2 &get_currentRootAnchorPPosOff() const {
        return actionPtr->framePoses.at(actionHandle.currentIdx).get_rootAnchorPos().pposOff;
    }


    //------- action -------
    //  ... 临时设置为公共 ...
    // 具象go类代码 通过 name／id 来 设置／改写 action数据
    // 在单一时间，一个GameMesh实例 只能绑定 一个 action实例
    std::string  actionName;       //- 与下方的 actionPtr 强关联 --
    Action      *actionPtr {nullptr}; //- 指向 esrc::actions 中的某个 action 实例
                                    //- 通过 bind_action() 来绑定

    ActionHandle actionHandle {}; //- 一个 GameMesh拥有一个 ah实例
                                    //- 由于 ah实例 只存在于mem态，所以几乎很少存在 反射的需求。
                                    //- 但是存在 类型验证的需求：通过 .typeId 
    
    ChildMesh   picMesh    { true };
    ChildMesh   shadowMesh { false };


    bool   isVisible  {true};  //- 是否可见 ( go and shadow )    
    bool   isCollide  {true};  //- 本mesh所拥有的 碰撞区 是否参与 碰撞检测


private:
    GameObj  *goPtr {nullptr}; //- 每个 gameMesh实例 都属于一个 go实例. 强关联
                               
};



#endif

