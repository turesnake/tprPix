/*
 * ========================= Model.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.08.24
 *                                        修改 -- 2018.08.24
 * ----------------------------------------------------------
 *   3D模型 Model 类
 *   旧实现，后期会被 新模块 取代
 * ----------------------------
 */
#ifndef _TPR_MODEL_H_
#define _TPR_MODEL_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include <glad/glad.h>  

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
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
#include <string>
#include <vector>

//------------------- SELF --------------------//
#include "Texture.h"
#include "ShaderProgram.h" //-- 每个 Model对象。都会绑定一个 着色器程序对象

#include "vector_matrix.h" 




class Model{

public:
    Model()
        //:
        {}

    //- 随着 RectVertics 的调用 被内置到 model/ new mesh 类中，
    //- 这个函数将被废弃。
    //- 由于数据是固定的，不再需要外部绑定，而是直接固定在 Mesh 类内
    inline void set_VBO( GLvoid *VBOdata, GLsizeiptr VBOsize, GLsizei stride_ )
                {  
                    VBO_data = VBOdata;
                    VBO_size = VBOsize;
                    stride = stride_;
                }


    inline void set_shader_program( ShaderProgram * const sp )
                {
                    shader_p = sp;
                }
    
    inline void set_translate( const glm::vec3 &v )
                {
                    if( is_model_change != true ){
                        is_model_change = true;
                    }
                    translate_val = v;
                }

    //- pix游戏 也需要 旋转 --
    inline void set_rotate( const glm::vec3 &v )
                {
                    if( is_model_change != true ){
                        is_model_change = true;
                    }
                    rotate_val = v;
                }

    inline void set_scale( const glm::vec3 &v )
                {
                    if( is_model_change != true ){
                        is_model_change = true;
                    }
                    scale_val = v;
                }

    void add_texture( Texture &tex );

    
    void init();
    void model_draw();
    void model_delete(); 

    
    inline size_t nr_of_textures() const 
                    { return textures.size(); }

    //-- 暂时未启用
    // inline glm::mat4 &get_mat4_model() { return mat4_model; }


private:
    GLuint VAO = 0; //- obj
    GLuint VBO = 0; //- obj

    GLvoid *VBO_data; //- 在本游戏中，固定为 &(rect_base[0])
                      //- 一个 2*2像素 的固定矩形。
    GLsizeiptr VBO_size;  //-- sizeof(VBO_data)
    GLsizei stride; //-- 步长，glVertexAttribPointer 函数中用到

    //-- 一个模型拥有数个 贴图文件, 动态添加
    std::vector<Texture> textures;

    //-- 本 Model对象 绑定的 着色器程序对象 指针
    ShaderProgram *shader_p  {nullptr}; 

    
    //+++++++++ 与 model 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model; //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵

    //--- 本 model对象 是否发生 位移／旋转／缩放 上的改变
    //-- 若为 true，表示 改变了，需要 更新 model矩阵,
    //-- 若为 false，表示 没改变，不需要 更新 model矩阵,
    //-- 这组 状态值 会影响函数 update_mat4_model 的逻辑。
    bool is_model_change = false;

    //-- 位移／旋转／缩放 变化向量。
    //-- 更新任一向量，都需要将 is_model_change 设为 true。
    glm::vec3 translate_val; 
    glm::vec3 rotate_val; //-- （角度）
    glm::vec3 scale_val { glm::vec3(1.0f, 1.0f, 1.0f) };  

    //+++++++++++ anchor ／ 锚点 +++++++++++++
   

    //--------- funcs ----------
    void update_mat4_model(); //-- 重新计算 model矩阵


};





#endif


