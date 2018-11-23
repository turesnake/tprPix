/*
 * ========================= ShaderProgram.cpp ==========================
 *                          -- GL_CPP --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   着色器程序 类
 * ----------------------------
 */
#include "ShaderProgram.h"

#include<GLFW/glfw3.h>

//-------------------- C ----------------------//
#include <cassert> //-- assert。
#include <iostream>
#include <stdio.h> // printf

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- SELF --------------------//
#include "global.h"


using std::string;
using std::cout;
using std::endl;

using tpr::path_combine;
using tpr::file_load;


//-------- 初始化 静态数据成员 ----------
GLuint ShaderProgram::shaderProgram_current = 0;


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void ShaderProgram::init(){

    int success;
    char infoLog[512]; //-- 出错信息 暂存 buf

    //-- vs/fs 文件的 数据存储地
    string vsbuf;
    string fsbuf;

    //-- 读取文件，获得 数据
    file_load( path_combine(path_cwd, lpath_vs).c_str(), vsbuf );
    file_load( path_combine(path_cwd, lpath_fs).c_str(), fsbuf );
    //-------------------
    int v_shader = glCreateShader( GL_VERTEX_SHADER );
    int f_shader = glCreateShader( GL_FRAGMENT_SHADER );

    compile( v_shader, vsbuf, "vertexShader" );
    compile( f_shader, fsbuf, "fragmentShader" );

    program = glCreateProgram();
    glAttachShader( program, v_shader );
    glAttachShader( program, f_shader );
    glLinkProgram( program );

    //--- 检查 是否创建成功 ---
    glGetProgramiv( program, GL_LINK_STATUS, &success );
    if( !success ){
        glGetProgramInfoLog( program, 512, NULL, infoLog );
        cout << "ShaderProgram::init: error: \n" 
             << infoLog << endl;
        assert(0);
    }
    //--- 及时释放 ---
    glDeleteShader( v_shader );  
    glDeleteShader( f_shader );
}


/* ===========================================================
 *                         compile
 * -----------------------------------------------------------
 * -- param: _shaderObj --
 * -- param: _sbuf  -- 存放 着色器程序文本的 buf
 * -- param: _sname -- 着色器程序名，比如 “vertexShader”
 */
void ShaderProgram::compile( int _shaderObj, const string &_sbuf, const string &_sname ){

    int success;
    char infoLog[512]; //-- 出错信息 暂存 buf

    //---------- 编译 ---------
    const char *sbufp = _sbuf.c_str(); //-- 转换为 c风格字符串指针。

    glShaderSource( _shaderObj, 1, (const GLchar **)&sbufp, NULL );
    glCompileShader( _shaderObj );

    //-------- 检查 编译 是否成功 ---------
    glGetShaderiv( _shaderObj, GL_COMPILE_STATUS, &success );
    if( !success ){
        glGetShaderInfoLog( _shaderObj, 512, NULL, infoLog );
        cout << "ShaderProgram::compile: \n" << _sname
             << "     error:\n"
             << infoLog << endl;
        assert(0);
    }
}

/* ===========================================================
 *                      use_program
 * -----------------------------------------------------------
 * -- 包裹函数，
 * -- 这个函数可以被 大量使用，而不会有 性能损耗。
 */
void ShaderProgram::use_program(){

    if( program == shaderProgram_current ){
        return;
    }
    glUseProgram( program );
    shaderProgram_current = program;
}

/* ===========================================================
 *                   get_uniform_location
 * -----------------------------------------------------------
 * -- glGetUniformLocation 的 包裹函数
 * -- 通过枚举类型 来 指定 操作的 着色器变量 类型。
 * -- 第二版，通过一个 unordered_map 来 动态管理
 */
void ShaderProgram::get_uniform_location( const string &name ){

    //-- 确保不重复设置 --
    assert( uniform_vals.find( name ) == uniform_vals.end() );
    //-- 获得地址。
    GLuint location = glGetUniformLocation( program, name.c_str() );
    //-- 添加到 map 中
    uniform_vals.insert( { name, location } );
}

/* ===========================================================
 *                   uniform_location
 * -----------------------------------------------------------
 * -- 获得 目标变量的 地址值。临时函数
 * -- 通过枚举类型 来 指定 着色器变量 类型。
 * -- 第二版，通过一个 unordered_map 来 动态管理
 */
GLuint ShaderProgram::uniform_location( const string &name ){

    //-- 获得 map 元素的 迭代器，类型为一个 pair
    auto map_it = uniform_vals.find( name ); 
    if( map_it == uniform_vals.end() ){
        cout << "\nuniform_location: error:\n"
            << "    no such val : " << name
            << endl;
        assert(0);
    }
    return map_it->second;
}

/* ===========================================================
 *                 send_mat4_model_2_shader
 * -----------------------------------------------------------
 */
void ShaderProgram::send_mat4_model_2_shader( const glm::mat4 &m ){

    use_program();
    glUniformMatrix4fv( uniform_location( "model" ),
                        1, 
                        GL_FALSE, 
                        (const GLfloat*)(glm::value_ptr( m ))
                        );
}

/* ===========================================================
 *                send_mat4_view_2_shader
 * -----------------------------------------------------------
 */
void ShaderProgram::send_mat4_view_2_shader( const glm::mat4 &v ){

    use_program();
    glUniformMatrix4fv( uniform_location( "view" ),
                        1, 
                        GL_FALSE, 
                        (const GLfloat*)(glm::value_ptr( v ))
                        );
}

/* ===========================================================
 *              send_mat4_projection_2_shader
 * -----------------------------------------------------------
 */
void ShaderProgram::send_mat4_projection_2_shader( const glm::mat4 &p ){

    use_program();
    glUniformMatrix4fv( uniform_location( "projection" ),
                        1, 
                        GL_FALSE, 
                        (const GLfloat*)(glm::value_ptr( p ))
                        );
}



