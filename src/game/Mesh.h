/*
 * ========================= Mesh.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   Mesh 类, 可以看作 图元类。
 *   一个 mesh，负责管理 一张 矩形图元
 *  ----------
 * Mesh 和 Action_SRC 的区别：
 *	  mesh 的主体部分 就是原本的 model类
 *	  mesh 需要 图片资源，
 *	  而大部分 图片资源，以 Action_SRC 的形式存储。
 *  ----------
 *   一个 mesh 拥有：
 *     一个 根锚点 ／ root anchor   -- 代表 mesh 本身
 *     数个 子锚点 ／ child anchor  -- 用来绑定其他 meshes
 *  
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

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- SELF --------------------//
//#include "Action_SRC.h"
#include "vector_matrix.h"
#include "ShaderProgram.h" //-- 每个 Model对象。都会绑定一个 着色器程序对象



//-- 和 go类一样，Mesh类也是一个 "半伪接口类" --
//   在最终由用户定义的 “具象go类” 中，将创建 go／mesh实例
//   并对其做 具象配置和绑定。 
class Mesh{

public:
    explicit Mesh() = default;

    void init();
    void mesh_draw();
    void mesh_delete(); 



    u64   id;     //- go实例 在程序中的 主要搜索依据。

    //--- pos ---
    glm::vec2  pos; //- 以go实例 根锚点 为 0点的 相对pos位移
                    //  用来记录，本mesh 在 go中的 位置（图形）
                    //  大部分情况下（尤其是只有一个 mesh的 go实例），此值为 0
    float      z;   //- 一个 go实例 可能拥有数个 mesh，相互间需要区分 视觉上的 前后顺序
                    //- 此处的 z 值只是个 相对偏移值。比如，靠近摄像机的 mesh z +0.1f
                    //- 这个值 多数由 具象go类 填入。

    GLuint  *texNameptr {nullptr}; //- 指向 texName 的指针。
                    // texName 被存储在 action_src实例中，
                    // 在 具象go类，负责 动画帧调度的函数代码中，被动态绑定到此处
                    //-- tex 会在每次 draw 时 才被绑定，这正是我们想要的

    //--------------------------//

    inline void set_shader_program( ShaderProgram *sp ){
        shader_p = sp;
    }

    inline void set_translate( const glm::vec3 &v ){
        translate_val = v;
    }

    //- pix游戏 暂时只支持 轴旋转 --
    inline void set_rotate_z( float _z ){
        rotate_z = _z;
    }

    inline void set_scale( const glm::vec3 &v ){
        scale_val = v;
    }

    




private:
    GLuint VAO  {0}; //- obj id
    GLuint VBO  {0}; //- obj id


    //-- 本 Model对象 绑定的 着色器程序对象 指针
    ShaderProgram  *shader_p  {nullptr}; 


    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model {}; //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵

    //-- 本 model对象 是否发生 位移／旋转／缩放 上的改变
    //   若为 true，表示 改变了，需要 更新 model矩阵,
    //   若为 false，表示 没改变，不需要 更新 model矩阵,
    //-- 这组 状态值 会影响函数 update_mat4_model 的逻辑。
    //-- 目前尚未投入使用：
    //-- 由于每一帧 都需要 设置 scale值（将图元从 2*2 扩展为目标大小）
    //   这个值似乎很难用上。或者，可以做些试验
    bool is_model_change  {false};


    //-- 位移／旋转／缩放 变化向量。
    //-- 更新任一向量，都需要将 is_model_change 设为 true （未实现）
    glm::vec3 translate_val {};    //- 位移
    float     rotate_z    {0.0f};  //- z轴旋转角度
    glm::vec3 scale_val  {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 缩放比例（用半径来缩放）



    //--------- funcs ----------
    void update_mat4_model(); //-- 重新计算 model矩阵

};




#endif

