/*
 * ========================= GPGPU.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.03.28
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "GPGPU.h"

//-------------------- C ----------------------//
#include <cassert>

//-------------------- CPP ----------------------//
#include <string>
#include <iostream>
#include <vector>

//-------------------- lib ----------------------//
#include "tprFileSys.h" 
#include "tprDataType.h" 

//-------------------- Engine ----------------------//
//#include "global.h"
#include "srcs_engine.h"
#include "FRGB.h"


namespace{ //------------ namespace ------------//

    //  这组顶点数据 正好覆盖 整个 window [-1.0, 1.0] --
    //  中心坐标系
    std::vector<float> gpgpuVertices {
        // 坐标 [-1.0, 1.0]  |  textuel [0.0, 1.0] 
        -1.0f,  -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f,  -1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,   1.0f, 0.0f,   1.0f, 1.0f,

         1.0f,   1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f,   1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f,  -1.0f, 0.0f,   0.0f, 0.0f
    };


} //------------ namespace: end ------------//


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 *  一次性完成 gpgpu 实例的全部建设工作
 *  在此之后，还需要用户 手动添加 uniforms 
 */
void GPGPU::init(const IntVec2 &_size, 
                const std::string &_lpath_vs,
                const std::string &_lpath_fs ){

    //-------- texSize ---------
    this->texSize = _size; // must before FBO
    
    //--- 创建一个新的 frame-buffer-obj ---
    glGenFramebuffers( 1, &this->FBO );
    //-- 激活 gpgpuFBO，绑定到 fb-target：GL_FRAMEBUFFER （兼容draw，read）
    glBindFramebuffer( GL_FRAMEBUFFER, this->FBO );
            //- 在此绑定之后，所有的 读取／写入 操作，都会作用在这个 FBO 上
            // 此时，FBO 还不完整，需要：
            // -- 附加至少一个缓冲（颜色、深度或模板缓冲）
            // -- 至少有一个颜色附件(Attachment)
            // -- 所有的附件都必须是完整的（保留了内存）
            // -- 每个缓冲都应该有相同的样本数

    this->init_texture();
    //-- 检查 fbo 是否建设完毕 --
    assert( glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE );

    //-------- shaderProgram ---------
    this->ShaderProgram.init( _lpath_vs, _lpath_fs );
    this->ShaderProgram.use_program();
    this->ShaderProgram.add_new_uniform( "texture1" );
    glUniform1i( this->ShaderProgram.get_uniform_location("texture1"), 0); 
                                        //- 写入的值就是 0. 一次性设置，以后也不用再改写
                                        //  剩余的 uniform 需要在此函数之后，由用户程序 添加
    //-------- model ---------
    this->init_model();

    this->is_binded = true;
}


/* ===========================================================
 *                   init_texture
 * -----------------------------------------------------------
 * -- 目前版本，默认 texBUf 为空（或全为初始值）
 *  gpgpu运算所依赖的数据，全部通过 uniform 传入 gl
 */
void GPGPU::init_texture(){

    /*
    std::vector<RGBA> texBuf;
    //--- 将 tex 数据 填成一个 纯色 ---
    RGBA   color_init { 15, 35, 120, 255 };
    size_t entSize = this->texSize.x * this->texSize.y;
    texBuf.resize( entSize );
    for( size_t i=0; i<entSize; i++ ){
        texBuf.at(i) = color_init;
    } 
    */
    //--------
    glGenTextures( 1, &this->texName );
    glBindTexture( GL_TEXTURE_2D, this->texName );

    glTexImage2D(GL_TEXTURE_2D,    //-- 指定纹理目标／target，
                0,                 //-- 多级渐远纹理的级别: 0: 基本级别
                GL_RGB,            //-- 希望把纹理储存为何种格式
                this->texSize.x,   //-- 纹理的宽度
                this->texSize.y,   //-- 纹理的高度
                0,                 //-- 总是被设为0（历史遗留问题
                GL_RGB,            //-- 源图的 格式
                //GL_UNSIGNED_BYTE,          //-- 源图的 数据类型(要做数据运算，u8_t 显然不适合)
                GL_FLOAT,
                //(const GLvoid*)&(texBuf.at(0)) ); //-- 图像数据，
                NULL);               //-- 图像数据，仅分配空间，而不填充数据（以后填充）

    //-- 未设置 环绕方式的 纹理过滤 的策略，因为用不到 --
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小时 纹理过滤 的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大时 纹理过滤 的策略
    
    //-- 将 新纹理 作为附件，附加到 fbo 上 --
    glFramebufferTexture2D( GL_FRAMEBUFFER,       //- 帧缓冲的目标（绘制、读取或者两者皆有）
                            GL_COLOR_ATTACHMENT0, //- 想要附加的附件类型, 正在附加一个颜色附件, 可以附加多个
                            GL_TEXTURE_2D,        //- 希望附加的纹理类型
                            this->texName,       //- 要附加的纹理本身
                            0);                   //- 多级渐远级别
}   


/* ===========================================================
 *                  init_model
 * -----------------------------------------------------------
 * -- gpgpu 只有一个 图元，和 视口 等尺寸（pix）
 */
void GPGPU::init_model(){

    glGenVertexArrays( 1, &this->VAO );
    glGenBuffers(      1, &this->VBO );
    //----
    glBindVertexArray( this->VAO );
    glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
    glBufferData(   GL_ARRAY_BUFFER, 
                    (gpgpuVertices.size() * sizeof(float)),
                    (const GLvoid*)&(gpgpuVertices.at(0)), 
                    GL_STATIC_DRAW );

    GLsizei  stride = (GLsizei)( sizeof(float) * 5 );
    //-- 将 VAO 的 0号顶点属性，绑定到 VBO数据 的 position 段 
    //  对应 着色器 “vs文件” 中的: layout (location = 0) in vec3 aPos; 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    //-- 将 VAO 的 1号顶点属性，绑定到 VBO数据 的 texture 段
    //  对应 着色器 “vs文件” 中的: layout (location = 1) in vec2 aTexCoord;
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)) ); 
    glEnableVertexAttribArray( 1 ); 
}


/* ===========================================================
 *                      read_texFBuf
 * -----------------------------------------------------------
 */
void GPGPU::read_texFBuf(){

    this->texFBuf.clear();
    this->texFBuf.resize( this->texSize.x * this->texSize.y );
    //-----
    glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glReadPixels(   0,
                    0,
                    this->texSize.x,
                    this->texSize.y,
                    GL_RGB,
                    //GL_UNSIGNED_BYTE,
                    GL_FLOAT,
                    (void*)&(this->texFBuf.at(0)) );
}


/* ===========================================================
 *                    let_gpgpuFBO_work
 * -----------------------------------------------------------
 */
void GPGPU::let_gpgpuFBO_work(){

    assert( this->is_binded );

    //--- 更改渲染 视口，匹配数据尺寸 ---
    glViewport( 0, 0, this->texSize.x, this->texSize.y );
    //--------- 重绘背景 ---------
    glClearColor( 0.25f, 0.24f, 0.25f, 1.0f ); //- 随便某个颜色
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                //-- 清除上一帧的 颜色缓冲 和 深度缓冲
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //--------------------------//
    //    读取 gpu数据 到 mem
    //--------------------------//
    this->read_texFBuf();

    //--------------------------//
    //     切换回 regularFBO
    //--------------------------// 
    this->release();
    glViewport( 0, 0, esrc::windowFrameBufferWH.x, esrc::windowFrameBufferWH.y );
    glClearColor( 0.25f, 0.24f, 0.25f, 1.0f ); //- 随便某个颜色
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
                //-- 清除上一帧的 颜色缓冲 和 深度缓冲
}

