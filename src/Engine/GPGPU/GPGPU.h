/*
 * ========================= GPGPU.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.03.28
 *                                        修改 -- 
 * ----------------------------------------------------------
 *  gpgpu 模块
 * ----------------------------
 */
#ifndef _TPR_GPGPU_H_
#define _TPR_GPGPU_H_
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <string>
#include <unordered_map>

//------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "FRGB.h"
#include "ShaderProgram.h"

  
//----------- manual ------------//
//   GPGPU  gpgpu {};
//   gpgpu.init(...);
//   gpgpu.add_new_uniform(...);
//   gpgpu.release(); //- MUST !!!
//   ...
//   gpgpu.bind(); //- MUST !!!
//   "set uniforms" -- get_uniform_location()
//   gpgpu.let_gpgpuFBO_work();
//   "get datas" -- get_texFBuf();
//   gpgpu.release(); //- MUST !!!
class GPGPU{
public:
    GPGPU() = default;

    void init(  const IntVec2 &_size, 
                const std::string &_lpath_vs,
                const std::string &_lpath_fs  );

    void let_gpgpuFBO_work(); //- 核心程序


    inline void add_new_uniform( const std::string &_name ){
        assert( this->is_binded );
        this->ShaderProgram.add_new_uniform( _name );
    }

    inline GLuint get_uniform_location( const std::string &_name ){
        assert( this->is_binded );
        return this->ShaderProgram.get_uniform_location( _name );
    }

    inline void use_shaderProgram(){
        this->ShaderProgram.use_program();
    }

    inline const std::vector<FRGB> &get_texFBuf() const {
        return texFBuf;
    }

    //-- 统一绑定 IMPORTENT ---
    inline void bind(){
        glBindFramebuffer( GL_FRAMEBUFFER, this->FBO );
        this->ShaderProgram.use_program();
        glBindTexture(GL_TEXTURE_2D, this->texName);
        glBindVertexArray( this->VAO );
        this->is_binded = true;
    }

    //-- 统一释放 IMPORTENT ---
    inline void release(){
        glBindFramebuffer( GL_FRAMEBUFFER, 0 ); //--- 激活 regularFBO ---
        glBindTexture(GL_TEXTURE_2D, 0 );    //--- 释放 texture 绑定 ---
        glBindVertexArray( 0 );              //--- 释放 VAO 绑定 ---
        glBindBuffer( GL_ARRAY_BUFFER, 0 );  //--- 释放 VBO 绑定 ---
        this->is_binded = false;
    }


private:
    void init_texture();
    void init_model();
    void read_texFBuf();

    //===== vals =====//
    GLuint  FBO {};
    GLuint  VAO {};
    GLuint  VBO {};
    GLuint  texName {};

    IntVec2 texSize {};

    ShaderProgram  ShaderProgram {};
            //- GPGPU 实例 往往是独一无二的，比如实现 landwater分布的。
            //  这些实例，各自配有独立的 glsl 程序组。

    std::vector<FRGB> texFBuf; //- (4*float) 暂只支持这一种 tex数据类型
                                // 从 gpu 读取到的数据，存储在此

    //===== flags =====//
    bool  is_texBuf_NULL {true}; //- true:  向 gl 传输 空的 tex数据
                                 //- false: 向 gl 传输 用户自定义tex数据 
    bool  is_shaderProgram_set {false};
    bool  is_binded {false};;    //- 统一 绑定／释放

};



#endif 

