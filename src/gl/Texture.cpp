/*
 * ========================= Texture.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   texture / 材质 ／ 纹理 ／ 贴图
 * ----------------------------
 */

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //-- 加载图片数据用

#include <cassert>
#include <iostream>

#include "global.h"


using std::string;
using std::cout;
using std::endl;



/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 * -- 
 */
void Texture::init(){

    //--- 确保 文件的 相对路径名 以 ‘／’ 开头。
    char c = lpath_img[0];
    if( c != '/' ){
        cout << "\nTexture::init: error: need '/' " << endl;
        cout << "           lpath_img = " << lpath_img << "\n" << endl;
        assert(0);
    }

    //-- 合成 文件的 绝对路径名
    string path_img = path_cwd + lpath_img;

    glGenTextures(     1, &textel_name );
    glBindTexture( GL_TEXTURE_2D, textel_name );

    //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                    //-- GL_NEAREST 临近过滤，8-bit 专用
                                                    //-- GL_LINEAR  线性过滤，

    //--------------------------------------------//
    //             加载 获得 图片数据
    //--------------------------------------------//
    int width;
    int height;
    int nrChannels;
    unsigned char *data; //-- 临时，图片数据的指针。

    stbi_set_flip_vertically_on_load( 1 ); //-- 防止 图片 y轴颠倒。
    data = stbi_load( path_img.c_str(), &width, &height, &nrChannels, 0 );
    if( !data ){
        //err_sys("model_2_init: stbi_load: error.");
        cout << "Texture::init: stbi_load: error." << endl;
        assert(0);
    }

    //-- 通过之前的 png图片数据，生成 一个 纹理。
    glTexImage2D( GL_TEXTURE_2D,    //-- 指定纹理目标／target，
                    0,              //-- 多级渐远纹理的级别: 0: 基本级别
                    GL_RGB,         //-- 希望把纹理储存为何种格式
                    width,          //-- 纹理的宽度
                    height,         //-- 纹理的高度
                    0,              //-- 总是被设为0（历史遗留问题
                    format,         //-- 源图的 格式
                    GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                    data               //-- 图像数据
                    );
    
    //-- 8-bit 游戏不需要 多级渐远 
    //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成 多级渐远纹理
    //-- 释放 png图片 数据。
    stbi_image_free( data );

}




