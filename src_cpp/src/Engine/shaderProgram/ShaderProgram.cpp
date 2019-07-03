/*
 * ========================= ShaderProgram.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ShaderProgram.h"

//------------------- Libs --------------------//
#include "tprGeneral.h"

#include "SysConfig.h" // MUST BEFORE TPR_OS_WIN32_ !!!
#if defined TPR_OS_WIN32_ 
    #include "tprFileSys_win.h"
#elif defined TPR_OS_UNIX_ 
    #include "tprFileSys_unix.h"
#endif


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"

//#include "tprDebug.h" //- tmp
#include <iostream>


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

    //-- vs/fs bufs
    std::string vsbuf {};
    std::string fsbuf {};

    std::string path_vs = tprGeneral::path_combine(path_shaders, lpathVs_);
    std::string path_fs = tprGeneral::path_combine(path_shaders, lpathFs_);

    //-- read files --
#if defined TPR_OS_WIN32_
    tprWin::file_load( path_vs, vsbuf );
    tprWin::file_load( path_fs, fsbuf );
#elif defined TPR_OS_UNIX_
    tprUnix::file_load( path_vs, vsbuf );
    tprUnix::file_load( path_fs, fsbuf );
#endif

    //-------------------
    GLuint v_shader = glCreateShader( GL_VERTEX_SHADER );
    GLuint f_shader = glCreateShader( GL_FRAGMENT_SHADER );

    compile( v_shader, vsbuf );
    compile( f_shader, fsbuf );

    this->shaderProgram = glCreateProgram();
    glAttachShader( this->shaderProgram, v_shader );
    glAttachShader( this->shaderProgram, f_shader );
    glLinkProgram( this->shaderProgram );

    //--- check if compile is successed ---
    glGetProgramiv( this->shaderProgram, GL_LINK_STATUS, &success );
    if( !success ){
        glGetProgramInfoLog( this->shaderProgram, 512, nullptr, infoLog );
        //cout << "ShaderProgram::init: error: \n" 
        //     << infoLog << endl;
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
        std::cout << infoLog << std::endl;
        tprAssert(0);
    }
}

