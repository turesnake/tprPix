/*
 * ========================= Mesh.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   Mesh 类, 可以看作 图元类。
 *   一个 mesh，负责管理 一张 矩形图元
 *  ----------
 * Mesh 和 Action 的区别：
 *	  mesh 的主体部分 就是原本的 model类
 *	  mesh 需要 图片资源，
 *	  而大部分 图片资源，以 gl texName 的形式，存储在 Action 模块中。
 *  ----------
 *   一个 mesh 拥有：
 *     一个 根锚点 ／ root anchor   -- 代表 mesh 本身
 *     数个 子锚点 ／ child anchor  -- 用来绑定其他 meshes
 *  ----------
 *   和 go类一样，mesh只记录数据，具体的使用方式，由最终的 具象go类 区定义
 * 
 * 
 * ----------------------------
 */
#ifndef _TPR_MESH_H_
#define _TPR_MESH_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
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
#include "vector_matrix.h" 
#include "ShaderProgram.h" //-- 每个 Model对象。都会绑定一个 着色器程序对象
#include "VAOVBO.h" 
#include "ActionHandle.h"
#include "Action.h" 



//-- Mesh 是个 "整合类"，没有 具象类 --
// mesh 被轻量化了：
//  -- 不再单独管理自己的 VAO，VBO （改为使用全局唯一的 VAO，VBO）
//  -- texName 存储在 action 模块中。mesh 通过一个指针 调用它们
//    （调用方式由 具象go类 定义）
// --------
// 每个 具象go类实例 所拥有的 mesh实例，都将被单独存储 在 mem态。（比如存储在 具象go实例 内）
// 但是，mesh实例 不会被存入 硬盘态。
// 而是在每次 加载section中 具象go实例时，临时生成。
// --------
// Mesh实例拥有：
//  -1- actionPtr  实例本体 存储在 全局容器 actions 中。
//  -2- actionHandle实例（独占）
//  mesh 整合这两个数据，并实现最终的 draw() 函数
//
class Mesh{
public:
    Mesh() = default;

    void init();
    void draw();

    //--- pos ---
    glm::vec2  pos {}; //- 以go实例 根锚点 为 0点的 相对pos位移 
                    //  用来记录，本mesh 在 go中的 位置（图形）
                    //  大部分情况下（尤其是只有一个 mesh的 go实例），此值为 0
                    //--- 此值与 action 的 anchors_root 不同。
                    //  此值就是个单纯的 具象类定义值

    float      z {0.0f};   //- 一个 go实例 可能拥有数个 mesh，相互间需要区分 视觉上的 前后顺序
                    //- 此处的 z 值只是个 相对偏移值。比如，靠近摄像机的 mesh z +0.1f
                    //- 这个值 多数由 具象go类 填入的。

    ActionHandle actionHandle {}; //- 一个 mesh拥有一个 ah实例
                                    //- 由于 ah实例 只存在于mem态，所以几乎很少存在 反射的需求。
                                    //- 但是存在 类型验证的需求：通过 .typeId 
    
    bool   is_visible  {false}; //- 是否可见
                                //- 默认初始值 务必为 false
                                //- 可以避免没有 init完成的 mesh 被渲染

    //--------------------------//
    inline void set_shader_program( ShaderProgram *sp ){
        shaderPtr = sp;
    }

    //-- mesh 的 世界pos 与 go实例 强关联 --
    //-- 此函数 只能在 RenderUpdate 阶段被调用 --
    //-- 其余代码 不应随意调用 此函数!!! --
    inline void set_translate( const glm::vec2 &_goCurrentPos ){

        PixVec2 v = actionPtr->anchors_root[ actionHandle.currentIdx ];
        translate_val = glm::vec3{  _goCurrentPos.x + (float)pos.x - (float)v.x, 
                                    _goCurrentPos.y + (float)pos.y - (float)v.y, 
                                    _goCurrentPos.y + (float)pos.y - (float)v.y + z };
    }

    //- pix游戏 暂时只支持 轴旋转 --
    inline void set_rotate_z( float _z ){
        rotate_z = _z;
    }

    //- 外部手动改写 scale_val --
    inline void set_scale( const glm::vec3 &v ){
        scale_val = v;
    }

    //- 大部分 具象go实例 的 mesh图元 长宽值 与 action数据 强关联 --
    //  所以可以直接从 action 中获取数据
    //  这个函数很常用
    //  但如果 action实例 并不更换，也没必要 每1视觉帧 都执行此函数
    inline void set_scale_auto(){
        const PixVec2 &p = actionPtr->pixes_per_frame;
        scale_val = glm::vec3{ (float)p.x, (float)p.y, 1.0f };
    }


    //------ action ------
    void bind_action( const std::string &_name );
    inline const std::string &get_actionName() const {
        return actionName;
    }


private:
    //------- action -------
    // 具象go类代码 通过 name／id 来 设置／改写 action数据
    // 在单一时间，一个mesh实例 只能绑定 一个 action实例
    std::string  actionName;       //- 与下方的 actionPtr 强关联 --
    Action      *actionPtr {nullptr}; //- 指向 esrc::actions 中的某个 action 实例
                                    //- 通过 bind_action() 来绑定
    GLuint  texName {0}; //- 当前指向的 texName 值。
                    // texName 被存储在 action实例中，
                    // 这个变量只是为了 调用方便...

    //-- 本 Model对象 绑定的 着色器程序对象 指针
    ShaderProgram  *shaderPtr  {nullptr}; 

    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model {}; //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵

    //-- 位移／旋转／缩放 变化向量。
    //-- 更新任一向量，都需要将 is_model_change 设为 true （未实现）
    glm::vec3 translate_val {};    //- 位移
    float     rotate_z    {0.0f};  //- z轴旋转角度
    glm::vec3 scale_val  {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 缩放比例（用半径来缩放）

    //--------- funcs ----------
    void update_mat4_model(); //-- 重新计算 model矩阵
};




#endif

