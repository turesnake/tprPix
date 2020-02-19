/*
 * ========================= Mesh.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   A BASIC Mesh class.
 *   handle single texture.
 * ----------------------------
 */
#ifndef TPR_MESH_H
#define TPR_MESH_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h>  

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "ShaderProgram.h" //-- each Mesh instance,will bind a shader

#include "AnimFrameSet.h"    


class Mesh{
public:
    Mesh() = default;

    void init( GLuint texName_ );
    void draw();

    //--------------------------//
    inline void set_shader_program( ShaderProgram *sp_ )noexcept{
        this->shaderPtr = sp_;
    }
    inline void set_texName( GLuint texName_ )noexcept{
        this->texName = texName_;
    }
    inline void set_translate( const glm::vec3 &v_ )noexcept{
        this->translate_val = v_;
        this->isMat4Change = true;
    }
    inline void set_scale( const glm::vec3 &v_ )noexcept{
        this->scale_val = v_;
        this->isMat4Change = true;
    }
    
    //------- get -------//
    //- 通过 translate_val.z 值 来给 待渲染的 meshs 排序 --
    inline float get_render_z()const noexcept{ return this->translate_val.z; }

    //======== flags ========//
    bool   isVisible  {false};

private:
    void update_mat4_model(); //-- 重新计算 model矩阵

    //======== vals ========//
    GLuint          texName     {0};
    ShaderProgram  *shaderPtr   {nullptr}; 
    GLuint          VAO         {}; // simple copy from esrc_VAOVBO
    GLsizei         pointNums   {};   // glDrawArrays() param
    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model = glm::mat4(1.0); //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵
    //-- 位移／旋转／缩放 变化向量。
    glm::vec3 translate_val {};    
    glm::vec3 scale_val  {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 缩放比例（用半径来缩放）

    bool isMat4Change {true}; //- 位移／旋转／缩放 是否被改写。
                                //-  若被改写，就要重新计算 mat4_model   
};


#endif 

