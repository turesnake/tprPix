/*
 * ========================= GameMesh.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   A Mesh class DEEP bind to GameObj & Action.
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
 * 
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
#include "ShaderProgram.h" //-- each GameMesh instance,will bind a shader
#include "ActionHandle.h"
#include "Action.h" 



//-- GameMesh 是个 "整合类"，不存在它的 具象类 --
// GameMesh 被轻量化了：
//  -- 不再单独管理自己的 VAO，VBO （改为使用全局唯一的 VAO，VBO）
//  -- texName 存储在 action 模块中。GameMesh 通过一个指针 调用它们
//    （调用方式由 具象go类 定义）
// --------
// 每个 具象go类实例 所拥有的 GameMesh实例，都将被单独存储 在 mem态。（比如存储在 具象go实例 内）
// 但是，GameMesh实例 不会被存入 硬盘态。
// 而是在每次 加载section中 具象go实例时，临时生成。
// --------
// GameMesh实例拥有：
//  -1- actionPtr  实例本体 存储在 全局容器 actions 中。
//  -2- actionHandle实例（独占）
//  GameMesh 整合这两个数据，并实现最终的 draw() 函数
//
class GameMesh{
public:
    GameMesh() = default;

    void init();
    void draw();

    //--- pos ---
    glm::vec2  pos {}; //- 以go实例 根锚点 为 0点的 相对pos位移 
                    //  用来记录，本GameMesh 在 go中的 位置（图形）
                    //  大部分情况下（尤其是只有一个 GameMesh的 go实例），此值为 0
                    //--- 此值与 action 的 anchors_root 不同。
                    //  此值就是个单纯的 具象类定义值

    float      off_z {0.0f};   //- 一个 go实例 可能拥有数个 GameMesh，相互间需要区分 视觉上的 前后顺序
                    //- 此处的 off_z 值只是个 相对偏移值。比如，靠近摄像机的 GameMesh off_z +0.1f
                    //- 这个值 多数由 具象go类 填入的。

    ActionHandle actionHandle {}; //- 一个 GameMesh拥有一个 ah实例
                                    //- 由于 ah实例 只存在于mem态，所以几乎很少存在 反射的需求。
                                    //- 但是存在 类型验证的需求：通过 .typeId 
    
    bool   is_visible  {true}; //- 是否可见
                                //- 默认初始值 务必为 false
                                //- 可以避免没有 init完成的 GameMesh 被渲染

    //--------------------------//
    inline void set_shader_program( ShaderProgram *_sp ){
        shaderPtr = _sp;
    }

    //-- GameMesh 的 世界pos 与 go实例 强关联 --
    //-- 此函数 只能在 RenderUpdate 阶段被调用 --
    //-- 其余代码 不应随意调用 此函数!!! --
    inline void set_translate( const glm::vec2 &_goCurrentPos ){

        //- 图元左下角 到 anchor_root 的pos偏移 --
        PixVec2 v = actionPtr->anchors_root[ actionHandle.currentIdx ];
        translate_val = glm::vec3{  _goCurrentPos.x + (float)pos.x - (float)v.x, 
                                    _goCurrentPos.y + (float)pos.y - (float)v.y, 
                                    -(_goCurrentPos.y + (float)pos.y + off_z) }; 
                                        //-- ** 注意！**  z值的计算有不同：
                                        // -1- 取负...
                                        // -2- 没有算入 v.y; 因为这个值只代表：
                                        //     图元 和 根锚点的 偏移
                                        //     而 z值 仅仅记录 GameMesh锚点 在 游戏世界中的位置
    }

    //- pix游戏 暂时只支持 轴旋转 --
    inline void set_rotate_z( float _z ){
        rotate_z = _z;
    }

    //- 外部手动改写 scale_val --
    inline void set_scale( const glm::vec3 &_v ){
        scale_val = _v;
    }

    //- 大部分 具象go实例 的 GameMesh图元 长宽值 与 action数据 强关联 --
    //  所以可以直接从 action 中获取数据
    //  这个函数很常用
    //  但如果 action实例 并不更换，也没必要 每1视觉帧 都执行此函数
    inline void set_scale_auto(){
        const PixVec2 &p = actionPtr->pixes_per_frame;
        scale_val = glm::vec3{ (float)p.x, (float)p.y, 1.0f };
    }

    //- 通过 translate_val.z 值 来给 待渲染的 gameMeshs 排序 --
    inline float get_render_z() const {
        return translate_val.z;
    }

    //------ action ------
    void bind_action( const std::string &_name );
    inline const std::string &get_actionName() const {
        return actionName;
    }

    inline int get_frames() const {
        return actionPtr->texNames.size();
    }

private:
    //------- action -------
    // 具象go类代码 通过 name／id 来 设置／改写 action数据
    // 在单一时间，一个GameMesh实例 只能绑定 一个 action实例
    std::string  actionName;       //- 与下方的 actionPtr 强关联 --
    Action      *actionPtr {nullptr}; //- 指向 esrc::actions 中的某个 action 实例
                                    //- 通过 bind_action() 来绑定
    GLuint  texName {0}; //- 当前指向的 texName 值。
                    // texName 被存储在 action实例中，
                    // 这个变量只是为了 调用方便...

    ShaderProgram  *shaderPtr  {nullptr}; 

    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model {}; //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵

    //-- 位移／旋转／缩放 变化向量。
    glm::vec3 translate_val {};    
    float     rotate_z    {0.0f};  //- 只有 z轴旋转角度
    glm::vec3 scale_val  {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 缩放比例（用半径来缩放）


    //--------- funcs ----------
    void update_mat4_model(); //-- 重新计算 model矩阵
};




#endif

