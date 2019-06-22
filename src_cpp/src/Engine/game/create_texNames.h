/*
 * ==================== create_texNames.h ===================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CREATE_TEXNAMES_H_
#define TPR_CREATE_TEXNAMES_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "RGBA.h"


/* ===========================================================
 *                    create_texNames
 * -----------------------------------------------------------
 * param: _texNum      -- 要创建 几个 texName
 * param: _imgWH     -- 目标texture 的 宽度长度
 * param: _frame_data_ary  -- 图形数据本体
 * param: _texNamesBuf -- 最终生成的 texNames 存入此容器
 */
inline void create_texNames(size_t _texNum,
                            const IntVec2 &_imgWH,
                            const std::vector<std::vector<RGBA>> &_frame_data_ary,
                            std::vector<GLuint> &_texNamesBuf ){

    _texNamesBuf.resize( _texNum );
    //-- 申请 _texNum个 tex实例，并获得其 names
    glGenTextures( static_cast<GLsizei>(_texNum), &_texNamesBuf.at(0) );

    for( size_t i=0; i<_texNum; i++ ){

        glBindTexture( GL_TEXTURE_2D, _texNamesBuf.at(i) );
        //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                        //-- GL_NEAREST 临近过滤，8-bit 专用
                                                        //-- GL_LINEAR  线性过滤，
        const GLvoid *dptr = static_cast<const GLvoid*>(&(_frame_data_ary.at(i).at(0)));
        //-- 通过之前的 png图片数据，生成 一个 纹理。
        glTexImage2D( GL_TEXTURE_2D,       //-- 指定纹理目标／target，
                        0,                 //-- 多级渐远纹理的级别: 0: 基本级别
                        GL_RGBA,           //-- 希望把纹理储存为何种格式
                        _imgWH.x,        //-- 纹理的宽度
                        _imgWH.y,        //-- 纹理的高度
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
 * param: _imgWH     -- 目标texture 的 宽度长度
 * param: _imgDataPtr  -- 图形数据本体
 */
inline GLuint create_a_texName( const IntVec2 &_imgWH,
                             const GLvoid  *_imgDataPtr ){

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
                    _imgWH.x,        //-- 纹理的宽度
                    _imgWH.y,        //-- 纹理的高度
                    0,                 //-- 总是被设为0（历史遗留问题
                    GL_RGBA,           //-- 源图的 格式
                    GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                    _imgDataPtr        //-- 图像数据
                    );
    //-- 本游戏不需要 多级渐远 --
    //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    
    return texName;
}

/* ===========================================================
 *                    create_a_empty_texName
 * -----------------------------------------------------------
 * param: _imgWH     -- 目标texture 的 宽度长度
 */
inline GLuint create_a_empty_texName( const IntVec2 &_imgWH ){
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
                    _imgWH.x,         //-- 纹理的宽度
                    _imgWH.y,         //-- 纹理的高度
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

