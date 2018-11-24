/*
 * ========================= Model.cpp ==========================
 *                          -- GL_CPP --
 *                                        创建 -- 2018.08.24
 *                                        修改 -- 2018.08.24
 * ----------------------------------------------------------
 *   3D模型 Model 类
 * ----------------------------
 */

#include "Model.h"

//-------------------- CPP --------------------//
#include <cassert> //-- assert。
#include <iostream>

using std::cout;
using std::endl;


//---------------------- 局部 变量 ---------------------

//-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
const static glm::vec3 axle_x( 1.0f, 0.0f, 0.0f );
const static glm::vec3 axle_y( 0.0f, 1.0f, 0.0f );
const static glm::vec3 axle_z( 0.0f, 0.0f, 1.0f );


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- 
 */
void Model::init(){

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
    glBufferData( GL_ARRAY_BUFFER, VBO_size, VBO_data, GL_STATIC_DRAW );

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
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); 
                //-- 解绑 当前的 VBO。
                //-- 由于 glVertexAttribPointer 已经将 具体 VBO数据 绑定到 对应 VAO 上了
                //-- 所以，现在 解绑 VBO 是允许的。

    glBindVertexArray( 0 );             
                //-- 解绑 当前的 VAO。 最后一个执行。


    //---------------------------------------------//
    //                  texture
    //---------------------------------------------//
    //-- 初始化 所有 texture
    auto it = textures.begin();
    for( ; it != textures.end(); it++ ){
        it->init();
    }

}


/* ===========================================================
 *                  model_draw
 * -----------------------------------------------------------
 */
void Model::model_draw(){

    //---------- 重新计算 model矩阵 -------------
    update_mat4_model();

    //---------- 将 model矩阵的值传入 绑定的 着色器程序 ---------
    if( shader_p == nullptr ){
        cout << "Model::model_draw: error \n" 
             << "shader_p == nullptr" << endl; 
        assert(0); //-- 粗鲁地报错。
    }
    shader_p->send_mat4_model_2_shader( mat4_model );

    //----------- 绑定 本Model对象 的 所有 textures ------------
    size_t nr_textures = nr_of_textures();
    for( int i=0; i<nr_textures; ++i  ){
        glActiveTexture( GL_TEXTURE0 + i  );
        glBindTexture(GL_TEXTURE_2D, textures[i].get_textel_name() );
    }

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, 6 ); //-- 绘制 图元(rect). 不依赖 EBO 的写法
}


/* ===========================================================
 *                  model_delete
 * -----------------------------------------------------------
 */
void Model::model_delete(){

    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers(      1, &VBO );
}


/* ===========================================================
 *                  texture_add
 * -----------------------------------------------------------
 * -- 添加一个 texture ／纹理贴图
 */
void Model::add_texture( Texture &tex ){

    textures.push_back( tex ); //-- 这么写是否正确？

}


/* ===========================================================
 *                 update_mat4_model
 * -----------------------------------------------------------
 * -- 当 model对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 * -- 目前 优化粒度较粗。一有变化就全盘重算。
 */
void Model::update_mat4_model(){

    if( is_model_change == false ){
        return; 
    }
    is_model_change = false;  //--- 重置 标志符。


    //----- 位移：正常移动 ------
    mat4_model = glm::translate(
                            normal_mat4,
                            translate_val );

    //----- 旋转 ------
    //- pixel 游戏不支持旋转

    //----- 缩放 ------
    mat4_model = glm::scale( mat4_model, 
                            scale_val );


    //----- 位移：anchor／锚点 修正 ------

}

/* ===========================================================
 *                          test
 * -----------------------------------------------------------
 * -- 万用函数，检测用
 */
void Model::test(){

    float *cubep = (float*)VBO_data;


    cout << "\n\n----  "
                << cubep[ 0] << ", "
                << cubep[ 1] << ", "
                << cubep[ 2] << ", "
                << endl;


}















