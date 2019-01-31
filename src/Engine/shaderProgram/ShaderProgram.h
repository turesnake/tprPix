/*
 * ========================= ShaderProgram.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   着色器程序 类
 * ----------------------------
 */

#ifndef _TPR_SHADER_PROGRAM_H_
#define _TPR_SHADER_PROGRAM_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  

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
#include <unordered_map>

//------------------- Libs --------------------//
#include "nocopyable.h" 


class ShaderProgram : tpr::nocopyable {

public:
    explicit ShaderProgram( const std::string &_lpathVs, 
                            const std::string &_lpathFs 
                            )
        :
        lpath_vs( _lpathVs ),
        lpath_fs( _lpathFs )
        {}

    ~ShaderProgram() = default;

    //--- init ---//
    void init();

    //-- 从 着色器程序中 获得 目标变量的 地址
    void get_uniform_location( const std::string &name );
    //-- 获得 目标变量的 地址值
    GLuint uniform_location( const std::string &name );

    void use_program(); //-- glUserProgram 的 包裹函数

    //-- 将 3个矩阵 的值 传输到 着色器程序。
    void send_mat4_model_2_shader( const glm::mat4 &m ); //-- 主要由 Model 调用
    void send_mat4_view_2_shader( const glm::mat4 &v );
    void send_mat4_projection_2_shader( const glm::mat4 &p );


private:
    GLuint program = 0; 
    std::string lpath_vs; //-- 指针，指向 vertexShader 文件 相对路径名 字符串
    std::string lpath_fs; //-- 指针，指向 fragmentShader 文件 相对路径名 字符串

    //-- 着色器程序中的 uniform 变量们
    std::unordered_map<std::string, unsigned int> uniform_vals; 
                                            

    //------ 静态数据成员 ------- 
    static GLuint shaderProgram_current; //-- 当前被使用的 shaderProgram


    void compile( int _shaderObj, const std::string &_sbuf, const std::string &_sname );

};



#endif

