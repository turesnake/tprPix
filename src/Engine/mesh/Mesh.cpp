/*
 * ========================= Mesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   A BASIC Mesh class.
 *   handle single texture.
 * ----------------------------
 */
#include "Mesh.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "VAOVBO.h" 
#include "vector_matrix.h"

//#include "tprDebug.h" 

namespace{//------------------ namespace ---------------------//

    //-- z轴常量，用在 glm::rotate 函数中。
    const glm::vec3 axle_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace end ------------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void Mesh::init( GLuint _texName ){
    this->texName = _texName;
    this->isMat4Change = true;
}


/* ===========================================================
 *                  draw
 * -----------------------------------------------------------
 * -- 务必在 调用此函数前 设置 texName
 */
void Mesh::draw(){

    if( isVisible == false ){
        return;
    }

    //---------- refresh mat4_model -------------
    if( this->isMat4Change == true ){
        this->isMat4Change = false;
        update_mat4_model();
    }
    
    //---------- 将 model矩阵的值传入 绑定的 着色器程序 ---------
    assert( this->shaderPtr != nullptr );
    this->shaderPtr->send_mat4_model_2_shader( this->mat4_model );

    //----------- 绑定 本mesh实例 唯一的 texture ------------   
    //-- 单次 draw call 最多支持 32 个 texture。（完全够用）
    //   但是， gl本体可以存储 非常多个 tex实例
    glActiveTexture( GL_TEXTURE0 );  //- 激活纹理单元
    assert( this->texName != 0 ); 
    glBindTexture(GL_TEXTURE_2D, this->texName ); //- 绑定纹理单元

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, 6 ); //-- 绘制 图元(rect). 不依赖 EBO 的写法
}


/* ===========================================================
 *                 update_mat4_model
 * -----------------------------------------------------------
 * -- 当 mesh对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 */
void Mesh::update_mat4_model(){

    //----- translate: regular ------
    // 请确保，输入函数的 translate 值，已经叠加了 go 的 pos。
    this->mat4_model = glm::translate( normal_mat4, this->translate_val );

    //----- not support rotate yet ------

    //----- scale ------
    this->mat4_model = glm::scale( this->mat4_model, this->scale_val );

    //----- translate: anchor／锚点 修正 ------
    //...
}



