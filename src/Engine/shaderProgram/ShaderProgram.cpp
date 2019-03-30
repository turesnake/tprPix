/*
 * ========================= ShaderProgram.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   着色器程序 类
 * ----------------------------
 */
#include "ShaderProgram.h"

//-------------------- C ----------------------//
#include <cassert>

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//-------------------- Engine --------------------//
#include "global.h"

//#include "debug.h" //- tmp

//======== static ========//
GLuint ShaderProgram::shaderProgram_current {0};


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void ShaderProgram::init(   const std::string &_lpathVs, 
                            const std::string &_lpathFs ){

    int success;
    char infoLog[512]; //-- 出错信息 暂存 buf

    //-- vs/fs 文件的 数据存储地
    std::string vsbuf;
    std::string fsbuf;

    //-- 读取文件，获得 数据
    tpr::file_load( tpr::path_combine(path_shaders, _lpathVs).c_str(), vsbuf );
    tpr::file_load( tpr::path_combine(path_shaders, _lpathFs).c_str(), fsbuf );
    //-------------------
    GLuint v_shader = glCreateShader( GL_VERTEX_SHADER );
    GLuint f_shader = glCreateShader( GL_FRAGMENT_SHADER );

    compile( v_shader, vsbuf );
    compile( f_shader, fsbuf );

    this->shaderProgram = glCreateProgram();
    glAttachShader( this->shaderProgram, v_shader );
    glAttachShader( this->shaderProgram, f_shader );
    glLinkProgram( this->shaderProgram );

    //--- 检查 是否创建成功 ---
    glGetProgramiv( this->shaderProgram, GL_LINK_STATUS, &success );
    if( !success ){
        glGetProgramInfoLog( this->shaderProgram, 512, NULL, infoLog );
        //cout << "ShaderProgram::init: error: \n" 
        //     << infoLog << endl;
        assert(0);
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
 * -- param: _shaderObj --
 * -- param: _sbuf  -- 存放 着色器程序文本的 buf
 * -- param: _sname -- 着色器程序名，比如 “vertexShader”
 */
void ShaderProgram::compile( GLuint _shaderObj, const std::string &_sbuf ){

    int success;
    char infoLog[512]; //-- 出错信息 暂存 buf
    //---------- 编译 ---------
    const char *sbufPtr = _sbuf.c_str(); //-- 转换为 c风格字符串指针。
    glShaderSource( _shaderObj, 1, (const GLchar **)&sbufPtr, NULL );
    glCompileShader( _shaderObj );
    //-------- 检查 编译 是否成功 ---------
    glGetShaderiv( _shaderObj, GL_COMPILE_STATUS, &success );
    if( !success ){
        glGetShaderInfoLog( _shaderObj, 512, NULL, infoLog );
        assert(0);
    }
}

