/*
 * ========================= VAOVBO.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.24
 *                                        修改 -- 2018.12.24
 * ----------------------------------------------------------
 *   本游戏只有一份 VAO，VBO。
 *   所有的 mesh 都使用它
 * ----------------------------
 */
#include "VAOVBO.h"

//-------------------- C --------------------//
#include <cassert> //-- assert
//-------------------- CPP --------------------//
#include <vector>


namespace{//------------------ namespace ---------------------//

    //----------------------------//
    // 图元 定点数据 模版. 2*2 像素(半径扩展法)
    // 左下坐标系
    std::vector<float> rectBase {
        //      坐标            textuel 
        0.0f,  0.0f, 0.0f,  0.0f, 0.0f,
        1.0f,  0.0f, 0.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 0.0f,  0.0f, 0.0f
    };
    //- rectBase 的 总字节数
    GLsizeiptr VBO_size = (GLsizeiptr)(sizeof(float) * rectBase.size());
    //- 步长，glVertexAttribPointer 函数中用到
    GLsizei    stride = (GLsizei)( sizeof(float) * 5 );

}//--------------------- namespace end ------------------------//


/* ===========================================================
 *                      VAOVBO_init
 * -----------------------------------------------------------
 */
void VAOVBO_init(){

    //---------------------------------------------//
    //             申请 mod name
    //---------------------------------------------//
    glGenVertexArrays( 1, &VAO );
    glGenBuffers(      1, &VBO );

    //---------------------------------------------//
    //     创建 VBO，并将它们绑定到 对应的 VAO
    //---------------------------------------------//
    //-- VAO 的 target 是唯一的。绑定到 当前 VAO
    glBindVertexArray( VAO );

    //-- 制作一个 VBO，并设置好数据
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, 
                VBO_size, 
                (GLvoid*)&(rectBase[0]), 
                GL_STATIC_DRAW );

    //-- 将 VAO 的 0号顶点属性，绑定到 VBO数据 的 position 段 
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0 ); 
    glEnableVertexAttribArray( 0 ); 

    //-- 将 VAO 的 1号顶点属性，绑定到 VBO数据 的 texture 段
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)) ); 
    glEnableVertexAttribArray( 1 ); 

    //---------------------------------------------//
    //           解除绑定: VBO, VAO
    //---------------------------------------------//
    //-- 此时，VBO 调用的 那个 顶点数据 也可以被 释放了（gl自己复制了一份）
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); 
                //-- 解绑 当前的 VBO。
                //-- 由于 glVertexAttribPointer 已经将 具体 VBO数据 绑定到 对应 VAO 上了
                //-- 所以，现在 解绑 VBO 是允许的。

    glBindVertexArray( 0 );             
                //-- 解绑 当前的 VAO。 最后一个执行。
                //-- 目前来看，解不解绑都不要紧，反正全局只有一个 VAO
}


/* ===========================================================
 *                     VAOVBO_del
 * -----------------------------------------------------------
 */
void VAOVBO_del(){
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers(      1, &VBO );
}








