/*
 * ========================= VAOVBO.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "VAOVBO.h"



void VAOVBO::init(const GLvoid *pointsPtr_, GLsizeiptr  vboSize_ )noexcept{

    //---------------------------------------------//
    //             申请 mod name
    //---------------------------------------------//
    glGenVertexArrays( 1, &this->VAO );
    glGenBuffers(      1, &this->VBO );

    //---------------------------------------------//
    //     创建 VBO，并将它们绑定到 对应的 VAO
    //---------------------------------------------//
    //-- VAO 的 target 是唯一的。绑定到 当前 VAO
    glBindVertexArray( this->VAO );

    //-- 制作一个 VBO，并设置好数据
    glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
    glBufferData( GL_ARRAY_BUFFER, 
                vboSize_, 
                pointsPtr_, 
                GL_STATIC_DRAW );

    //---
    GLsizei  stride = static_cast<GLsizei>( sizeof(float) * 5 );
    //-- 将 VAO 的 0号顶点属性，绑定到 VBO数据 的 position 段 
    //  对应 着色器 “vs文件” 中的: layout (location = 0) in vec3 aPos; 
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, nullptr ); 
    glEnableVertexAttribArray( 0 ); 

    //-- 将 VAO 的 1号顶点属性，绑定到 VBO数据 的 texture 段
    //  对应 着色器 “vs文件” 中的: layout (location = 1) in vec2 aTexCoord;
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3*sizeof(float)) ); 
    glEnableVertexAttribArray( 1 ); 

    //---------------------------------------------//
    //        release bind: VBO, VAO
    //---------------------------------------------//
    //-- 此时，VBO 调用的 那个 顶点数据 也可以被 释放了（gl自己复制了一份）
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); 
                //-- 解绑 当前的 VBO。
                //-- 由于 glVertexAttribPointer 已经将 具体 VBO数据 绑定到 对应 VAO 上了
                //-- 所以，现在 解绑 VBO 是允许的。

    glBindVertexArray( 0 );
}

