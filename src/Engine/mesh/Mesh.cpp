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
#include "pch.h"
#include "Mesh.h" 

//-------------------- Engine --------------------//
#include "esrc_VAOVBO.h" 
#include "esrc_coordinate.h"

//namespace mesh_inn {//------------------ namespace: mesh_inn ---------------------//
//}//--------------------- namespace: mesh_inn end ------------------------//


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void Mesh::init( GLuint texName_ ){
    this->texName = texName_;
    this->isMat4Change = true;
    this->VAO = esrc::get_VAO();                // copy and hold
    this->pointNums = esrc::get_pointNums();    // copy and hold 
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
    tprAssert( this->shaderPtr != nullptr );
    this->shaderPtr->send_mat4_model_2_shader( this->mat4_model );

    //----------- 绑定 本mesh实例 唯一的 texture ------------   
    //-- 单次 draw call 最多支持 32 个 texture。（完全够用）
    //   但是， gl本体可以存储 非常多个 tex实例
    glActiveTexture( GL_TEXTURE0 );  //- 激活纹理单元
    tprAssert( this->texName != 0 ); 
    glBindTexture(GL_TEXTURE_2D, this->texName ); //- 绑定纹理单元

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( this->VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, this->pointNums );
}


/* ===========================================================
 *                 update_mat4_model
 * -----------------------------------------------------------
 * -- 当 mesh对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 */
void Mesh::update_mat4_model(){

    //----- translate: regular ------
    // 请确保，输入函数的 translate 值，已经叠加了 go 的 pos。

    //-- 修正：图元帧 左下角 -> 中心
    const auto &worldCoord = esrc::get_worldCoordRef();

    glm::dvec2 innDPos {    static_cast<double>( this->translate_val.x + this->scale_val.x * 0.5f ),
                            static_cast<double>( this->translate_val.y + this->scale_val.y * 0.5f ) };
    glm::dvec2 outDPos = worldCoord.calc_outDPos( innDPos );
    this->translate_val.x = static_cast<float>( outDPos.x );
    this->translate_val.y = static_cast<float>( outDPos.y );

    this->mat4_model = glm::translate( glm::mat4(1.0), this->translate_val );

    //----- not support rotate yet ------

    //----- scale ------
    this->mat4_model = glm::scale( this->mat4_model, this->scale_val );

    //----- translate: anchor／锚点 修正 ------
    //...
}



