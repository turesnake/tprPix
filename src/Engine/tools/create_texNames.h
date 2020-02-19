/*
 * ==================== create_texNames.h ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CREATE_TEXNAMES_H
#define TPR_CREATE_TEXNAMES_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "RGBA.h"


/* ===========================================================
 *                    create_texNames
 * -----------------------------------------------------------
 * param: texNum_      -- 要创建 几个 texName
 * param: imgWH_     -- 目标texture 的 宽度长度
 * param: frame_data_ary_  -- 图形数据本体
 * param: texNamesBuf_ -- 最终生成的 texNames 存入此容器
 */
inline void create_texNames(size_t  texNum_,
                            IntVec2 imgWH_,
                            const std::vector<std::vector<RGBA>> &frame_data_ary_,
                            std::vector<GLuint> &texNamesBuf_ ){

    texNamesBuf_.resize( texNum_ );
    //-- 申请 _texNum个 tex实例，并获得其 names
    glGenTextures( static_cast<GLsizei>(texNum_), &texNamesBuf_.at(0) );

    for( size_t i=0; i<texNum_; i++ ){

        glBindTexture( GL_TEXTURE_2D, texNamesBuf_.at(i) );
        //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                        //-- GL_NEAREST 临近过滤，8-bit 专用
                                                        //-- GL_LINEAR  线性过滤，
        const GLvoid *dptr = static_cast<const GLvoid*>(&(frame_data_ary_.at(i).at(0)));
        //-- 通过之前的 png图片数据，生成 一个 纹理。
        glTexImage2D( GL_TEXTURE_2D,       //-- 指定纹理目标／target，
                        0,                 //-- 多级渐远纹理的级别: 0: 基本级别
                        GL_RGBA,           //-- 希望把纹理储存为何种格式
                        imgWH_.x,        //-- 纹理的宽度
                        imgWH_.y,        //-- 纹理的高度
                        0,                 //-- 总是被设为0（历史遗留问题
                        GL_RGBA,           //-- 源图的 格式
                        GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                        dptr               //-- 图像数据
                        );
        //-- 本游戏不需要 多级渐远 --
        //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    }
}

/* ===========================================================
 *                    create_a_texName
 * -----------------------------------------------------------
 * param: imgWH_     -- 目标texture 的 宽度长度
 * param: imgDataPtr_  -- 图形数据本体
 */
inline GLuint create_a_texName( IntVec2 imgWH_,
                             const GLvoid  *imgDataPtr_ ){

    GLuint texName {};
    //-- 申请 _texNum个 tex实例，并获得其 names
    glGenTextures( 1, &texName );
    glBindTexture( GL_TEXTURE_2D, texName );
    //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                        //-- GL_NEAREST 临近过滤，8-bit 专用
                                                        //-- GL_LINEAR  线性过滤，
    //-- 通过之前的 png图片数据，生成 一个 纹理。
    glTexImage2D( GL_TEXTURE_2D,       //-- 指定纹理目标／target，
                    0,                 //-- 多级渐远纹理的级别: 0: 基本级别
                    GL_RGBA,           //-- 希望把纹理储存为何种格式
                    imgWH_.x,        //-- 纹理的宽度
                    imgWH_.y,        //-- 纹理的高度
                    0,                 //-- 总是被设为0（历史遗留问题
                    GL_RGBA,           //-- 源图的 格式
                    GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                    imgDataPtr_        //-- 图像数据
                    );
    //-- 本游戏不需要 多级渐远 --
    //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    
    return texName;
}

/* ===========================================================
 *                    create_a_empty_texName
 * -----------------------------------------------------------
 * param: imgWH_     -- 目标texture 的 宽度长度
 */
inline GLuint create_a_empty_texName( IntVec2  imgWH_ ){
    GLuint texName {};
    //-- 申请 _texNum个 tex实例，并获得其 names
    glGenTextures( 1, &texName );
    glBindTexture( GL_TEXTURE_2D, texName );
    //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                        //-- GL_NEAREST 临近过滤，8-bit 专用
                                                        //-- GL_LINEAR  线性过滤，
    //-- 通过之前的 png图片数据，生成 一个 纹理。
    glTexImage2D( GL_TEXTURE_2D,       //-- 指定纹理目标／target，
                    0,                 //-- 多级渐远纹理的级别: 0: 基本级别
                    GL_RGBA,           //-- 希望把纹理储存为何种格式
                    imgWH_.x,         //-- 纹理的宽度
                    imgWH_.y,         //-- 纹理的高度
                    0,                 //-- 总是被设为0（历史遗留问题
                    GL_RGBA,           //-- 源图的 格式
                    GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                    nullptr               //-- 图像数据, 为空
                    );
    //-- 本游戏不需要 多级渐远 --
    //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    return texName;
}



#endif 

