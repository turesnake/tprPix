/*
 * ========================= Mesh.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   Mesh 类, 可以看作 图元类。
 *   一个 mesh，负责管理 一张 矩形图元
 * ----------------------------
 */
#include "Mesh.h" 

//-------------------- C --------------------//
#include <cassert> //-- assert



namespace{//------------------ namespace ---------------------//

    //----------------------------//
    // 图元 定点数据 模版. 2*2 像素
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


    //-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
    //const glm::vec3 axle_x( 1.0f, 0.0f, 0.0f );
    //const glm::vec3 axle_y( 0.0f, 1.0f, 0.0f );
    const glm::vec3 axle_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace end ------------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void Mesh::init(){

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
    //--（ 注意，此时不能解除 EBO 的绑定。 ）
    //-- 此时，VBO 调用的 那个 顶点数据 也可以被 释放了
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); 
                //-- 解绑 当前的 VBO。
                //-- 由于 glVertexAttribPointer 已经将 具体 VBO数据 绑定到 对应 VAO 上了
                //-- 所以，现在 解绑 VBO 是允许的。

    glBindVertexArray( 0 );             
                //-- 解绑 当前的 VAO。 最后一个执行。

    //---------------------------------------------//
    //                  texName
    //---------------------------------------------//
    // texNameptr 不在此处被绑定...
}


/* ===========================================================
 *                  mesh_draw
 * -----------------------------------------------------------
 */
void Mesh::mesh_draw(){

    //---------- 重新计算 model矩阵 -------------
    update_mat4_model();

    //---------- 将 model矩阵的值传入 绑定的 着色器程序 ---------
    assert( shader_p != nullptr );
    shader_p->send_mat4_model_2_shader( mat4_model );

    //----------- 绑定 本mesh对象 唯一的 texture ------------    
    glActiveTexture( GL_TEXTURE0 );
    assert( texNameptr != nullptr );
    glBindTexture(GL_TEXTURE_2D, *texNameptr );

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, 6 ); //-- 绘制 图元(rect). 不依赖 EBO 的写法

}


/* ===========================================================
 *                  mesh_delete
 * -----------------------------------------------------------
 */
void Mesh::mesh_delete(){

    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers(      1, &VBO );
}




/* ===========================================================
 *                 update_mat4_model
 * -----------------------------------------------------------
 * -- 当 mesh对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 * -- 目前 优化粒度较粗。一有变化就全盘重算。
 */
void Mesh::update_mat4_model(){

    /*
    if( is_model_change == false ){
        return; 
    }
    is_model_change = false;  //--- 重置 标志符。
    */


    //----- 位移：正常移动 ------
    mat4_model = glm::translate(
                            normal_mat4,
                            translate_val );

    //----- 旋转 ------
    //- pix游戏 只支持 z轴旋转
    mat4_model = glm::rotate( mat4_model, 
                            glm::radians(rotate_z),
                            axle_z );


    //----- 缩放 ------
    mat4_model = glm::scale( mat4_model, 
                            scale_val );


    //----- 位移：anchor／锚点 修正 ------
    //...
}


