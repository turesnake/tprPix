/*
 * ========================= ShaderProgram.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   着色器程序 类
 * ----------------------------
 */
#ifndef TPR_SHADER_PROGRAM_H
#define TPR_SHADER_PROGRAM_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

//-------------------- Engine --------------------//
#include "UniformBlockObj.h"
#include "tprAssert.h"



class ShaderProgram{
public:
    ShaderProgram() = default;

    //--- init ---//
    void init(  const std::string &lpathVs_, 
                const std::string &lpathFs_ );

    //-- 从 着色器程序中 获得 目标变量的 地址
    inline void add_new_uniform( const std::string &name_ )noexcept{
        tprAssert( this->is_shaderProgram_set );
        auto [insertIt, insertBool] = uniforms.emplace( name_, glGetUniformLocation(this->shaderProgram, name_.c_str()) );
        tprAssert( insertBool );
    }

    inline GLint get_uniform_location( const std::string &name_ )noexcept{
        tprAssert( this->uniforms.find(name_) != this->uniforms.end() );
        return this->uniforms.at(name_);
    }

    inline void use_program()noexcept{
        if( this->shaderProgram == shaderProgram_current ){
            return;
        }
        glUseProgram( this->shaderProgram );
        shaderProgram_current = this->shaderProgram;
    }

    //-- 将 3个矩阵 的值 传输到 着色器程序。
    inline void send_mat4_model_2_shader( const glm::mat4 &m_ )noexcept{
        this->use_program();
        glUniformMatrix4fv( this->get_uniform_location( "model" ), 
                            1, 
                            GL_FALSE, 
                            glm::value_ptr(m_) );
    }

    inline GLuint get_shaderProgramObj()const noexcept{ return this->shaderProgram; }

private:
    void compile( GLuint shaderObj_, const std::string &sbuf_ );

    //======== vals ========//
    GLuint       shaderProgram  {0}; 

    //-- 着色器程序中的 uniform 变量们
    std::unordered_map<std::string, GLint> uniforms {}; 

    //======== flags ========//
    bool  is_shaderProgram_set {false};
                                            
    //======== static ========//
    static GLuint shaderProgram_current; //-- 当前被使用的 shaderProgram
};


#endif

