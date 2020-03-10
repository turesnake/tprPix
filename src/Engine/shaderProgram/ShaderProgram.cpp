/*
 * ========================= ShaderProgram.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "ShaderProgram.h"

//------------------- Libs --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"



//======== static ========//
GLuint ShaderProgram::shaderProgram_current {0};


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void ShaderProgram::init(   const std::string &lpathVs_, 
                            const std::string &lpathFs_ ){

    int success {};
    char infoLog[512]; //-- error buf

    //-- read files --
    std::string path_vs = tprGeneral::path_combine(path_shaders, lpathVs_);
    std::string path_fs = tprGeneral::path_combine(path_shaders, lpathFs_);
    auto vsbufUPtr = read_a_file( path_vs );
    auto fsbufUPtr = read_a_file( path_fs );

    //-------------------
    GLuint v_shader = glCreateShader( GL_VERTEX_SHADER );
    GLuint f_shader = glCreateShader( GL_FRAGMENT_SHADER );

    compile( v_shader, *vsbufUPtr );
    compile( f_shader, *fsbufUPtr );

    this->shaderProgram = glCreateProgram();
    glAttachShader( this->shaderProgram, v_shader );
    glAttachShader( this->shaderProgram, f_shader );
    glLinkProgram( this->shaderProgram );

    //--- check if compile is successed ---
    glGetProgramiv( this->shaderProgram, GL_LINK_STATUS, &success );
    if( !success ){
        glGetProgramInfoLog( this->shaderProgram, 512, nullptr, infoLog );
        tprDebug::console( "---- ShaderProgram::init: error: ----\n{}", infoLog );
        tprAssert(0);
    }
    //--- 及时释放 ---
    glDeleteShader( v_shader );  
    glDeleteShader( f_shader );
    //----------
    this->is_shaderProgram_set = true;
}


/* ===========================================================
 *                         compile
 * -----------------------------------------------------------
 * -- param: shaderObj_ --
 * -- param: sbuf_  -- 存放 着色器程序文本的 buf
 * -- param: _sname -- 着色器程序名，比如 “vertexShader”
 */
void ShaderProgram::compile( GLuint shaderObj_, const std::string &sbuf_ ){

    int success {};
    char infoLog[512]; //-- error buf
    //---------- 编译 ---------
    const char *sbufPtr = sbuf_.c_str();
    glShaderSource( shaderObj_, 1, (const GLchar **)&sbufPtr, nullptr );
    glCompileShader( shaderObj_ );
    //-------- check if compile is successed ---------
    glGetShaderiv( shaderObj_, GL_COMPILE_STATUS, &success );
    if( !success ){
        glGetShaderInfoLog( shaderObj_, 512, nullptr, infoLog );
        tprDebug::console( "{}", infoLog );
        tprAssert(0);
    }
}

