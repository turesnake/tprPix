/*
 * ========================= Canvas.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.03.31
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "Canvas.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "create_texNames.h"
#include "windowConfig.h"
#include "esrc_camera.h"


/* ===========================================================
 *                      init
 * -----------------------------------------------------------
 */
void Canvas::init(  IntVec2 *texSizePtr_,
                    const std::string &lpath_vs_,
                    const std::string &lpath_fs_ ){

    this->texSizePtr = texSizePtr_;

    //-------- shaderProgram ---------
    this->shaderProgram.init( lpath_vs_, lpath_fs_ );
    this->shaderProgram.use_program();
    this->shaderProgram.add_new_uniform( "model" );
    this->shaderProgram.add_new_uniform( "view" );
    this->shaderProgram.add_new_uniform( "projection" );
    this->shaderProgram.add_new_uniform( "texture1" );
        // 其它的 uniform 需要在此函数之后，由用户程序 添加

    //-------- mesh ---------
    this->mesh.init( create_a_empty_texName( *this->texSizePtr ) );
    this->mesh.isVisible = true;
    this->mesh.set_shader_program( &this->shaderProgram );
    this->mesh.set_scale(glm::vec3{ static_cast<float>(this->texSizePtr->x), 
                                    static_cast<float>(this->texSizePtr->y), 
                                    1.0f });
    this->is_binded = true;
}


/* ===========================================================
 *                       draw
 * -----------------------------------------------------------
 * 正确的 draw call 顺序：
 *   canvas.use_shaderProgram();
 *   "set uniforms" -- get_uniform_location()
 *   canvas.set_translate(...) //- MUST !!!
 *   canvas.draw();
 */
void Canvas::draw(){
    tprAssert( this->is_binded );

    //-- 每一帧都要 重设置，从而能让 canvas 跟随 被拉伸过的 window size --
    this->mesh.set_scale(glm::vec3{ static_cast<float>(this->texSizePtr->x), 
                                    static_cast<float>(this->texSizePtr->y), 
                                    1.0f });

    this->shaderProgram.send_mat4_view_2_shader( esrc::get_camera().update_mat4_view() );
    this->shaderProgram.send_mat4_projection_2_shader( esrc::get_camera().update_mat4_projection() );

    this->mesh.draw();
    this->is_binded = false;
}



