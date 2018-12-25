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

    //-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
    //const glm::vec3 axle_x( 1.0f, 0.0f, 0.0f );
    //const glm::vec3 axle_y( 0.0f, 1.0f, 0.0f );
    const glm::vec3 axle_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace end ------------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- 暂无内容...
 */
void Mesh::init(){
    //---------------------------------------------//
    //                  texName
    //---------------------------------------------//
    // texNamePtr 不在此处被绑定...
}


/* ===========================================================
 *                  mesh_draw
 * -----------------------------------------------------------
 */
void Mesh::mesh_draw(){

    //---------- 重新计算 model矩阵 -------------
    update_mat4_model();

    //---------- 将 model矩阵的值传入 绑定的 着色器程序 ---------
    assert( shaderPtr != nullptr );
    shaderPtr->send_mat4_model_2_shader( mat4_model );

    //----------- 绑定 本mesh对象 唯一的 texture ------------    
    glActiveTexture( GL_TEXTURE0 );  //- 激活纹理单元
    assert( texNamePtr != nullptr ); 
    glBindTexture(GL_TEXTURE_2D, *texNamePtr ); //- 绑定纹理单元

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, 6 ); //-- 绘制 图元(rect). 不依赖 EBO 的写法
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
    // 请确保，输入函数的 translate 值，已经叠加了 go 的 pos。
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


