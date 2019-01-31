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
#ifndef _TPR_MESH_H_
#define _TPR_MESH_H_
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

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "ShaderProgram.h" //-- each Mesh instance,will bind a shader

#include "AnimFrameIdxHandle.h"
#include "AnimFrameSet.h"    


class Mesh{
public:
    Mesh() = default;

    void init( GLuint _texName );
    void draw();

    bool   isVisible  {false};

    //--------------------------//
    inline void set_shader_program( ShaderProgram *_sp ){
        shaderPtr = _sp;
    }

    inline void set_texName( GLuint _texName ){
        texName = _texName;
    }

    inline void set_translate( const glm::vec3 &_v ){
        translate_val = _v;
        isMat4Change = true;
    }

    inline void set_scale( const glm::vec3 &_v ){
        scale_val = _v;
        isMat4Change = true;
    }

    //- 通过 translate_val.z 值 来给 待渲染的 meshs 排序 --
    inline float get_render_z() const {
        return translate_val.z;
    }

private:
    GLuint  texName {0};

    ShaderProgram  *shaderPtr  {nullptr}; 

    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model {}; //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵
    //-- 位移／旋转／缩放 变化向量。
    glm::vec3 translate_val {};    
    glm::vec3 scale_val  {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 缩放比例（用半径来缩放）

    bool isMat4Change {true}; //- 位移／旋转／缩放 是否被改写。
                                //-  若被改写，就要重新计算 mat4_model

    //--------- funcs ----------
    void update_mat4_model(); //-- 重新计算 model矩阵
};


#endif 

