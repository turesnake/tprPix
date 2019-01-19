/*
 * ====================== MapTexture.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2019.01.06
 *                                        修改 -- 2019.01.06
 * ----------------------------------------------------------
 *    texture for map sys
 * ----------------------------
 */
#include "MapTexture.h" 

//------------------- C --------------------//
#include <cassert>

//------------------- Engine --------------------//
#include "RGBA.h"

//----- static ------//
MapTexture::F_MAP_BUILDER  MapTexture::mapBuilder  {nullptr};


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 * -- before calling init(). 
 * -- Please bind MapTexture::mapBuilder FIRST
 */
void MapTexture::init(){

    glGenTextures(     1, &texName );
    glBindTexture( GL_TEXTURE_2D, texName );
    //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                    //-- GL_NEAREST 临近过滤，8-bit 专用
                                                    //-- GL_LINEAR  线性过滤，
    //---------------------------------------//
    //            mapTex build
    //---------------------------------------//
    texBuf.resize( pixSize.x * pixSize.y * sizeof(RGBA) );
    assert( MapTexture::mapBuilder != nullptr );
    
    MapTexture::mapBuilder( (u8*)&(texBuf.at(0)),
                            pixSize.x,
                            pixSize.y );
    
    //---------------------------------------//
    //        gl --> texName
    //---------------------------------------//
    glTexImage2D( GL_TEXTURE_2D,    //-- 指定纹理目标／target，
                    0,              //-- 多级渐远纹理的级别: 0: 基本级别
                    GL_RGBA,         //-- 希望把纹理储存为何种格式
                    pixSize.x,       //-- 纹理的宽度
                    pixSize.y,       //-- 纹理的高度
                    0,              //-- 总是被设为0（历史遗留问题
                    GL_RGBA,        //-- 源图的 格式
                    GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                    (u8*)&(texBuf.at(0))  //-- 图像数据
                    );
    //--- no need MipMap ---
    //-- 可以选择性释放 texBuf 资源 ---
    // 未定...
}


/* ===========================================================
 *                  bind_mapBuilder   [static]
 * -----------------------------------------------------------
 */
void MapTexture::bind_mapBuilder( F_MAP_BUILDER _fp ){
    mapBuilder = _fp;
}


