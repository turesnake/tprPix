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
    char infoLog[512]; //-- 出错信息 暂存 buf

    //-- vs/fs 文件的 数据存储地
    std::string vsbuf {};
    std::string fsbuf {};

    //-- 读取文件，获得 数据
#if defined TPR_OS_WIN32_
    tprWin::file_load( tprGeneral::path_combine(path_shaders, lpathVs_), vsbuf );
    tprWin::file_load( tprGeneral::path_combine(path_shaders, lpathFs_), fsbuf );
#elif defined TPR_OS_UNIX_
    tprUnix::file_load( tprGeneral::path_combine(path_shaders, lpathVs_), vsbuf );
    tprUnix::file_load( tprGeneral::path_combine(path_shaders, lpathFs_), fsbuf );
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

    //--- 检查 是否创建成功 ---
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
    char infoLog[512]; //-- 出错信息 暂存 buf
    //---------- 编译 ---------
    const char *sbufPtr = sbuf_.c_str(); //-- 转换为 c风格字符串指针。
    glShaderSource( shaderObj_, 1, (const GLchar **)&sbufPtr, nullptr );
    glCompileShader( shaderObj_ );
    //-------- 检查 编译 是否成功 ---------
    glGetShaderiv( shaderObj_, GL_COMPILE_STATUS, &success );
    if( !success ){
        glGetShaderInfoLog( shaderObj_, 512, nullptr, infoLog );
        std::cout << infoLog << std::endl;
        tprAssert(0);
    }
}

